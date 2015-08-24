#include<sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "lst_timer.h"

#define FD_LIMIT 65535//最大文件描述符
#define MAX_EVENT_NUMBER 1024//最大事件数
#define TIMESLOT 5//每次定时时间，相当于回搏时间

static int pipefd[2];//管道描述符用于将信号处理为统一事件源
static sort_timer_lst timer_lst;//定时器容器链表类
static int epollfd = 0;//epoll事件表描述符

int setnonblocking( int fd )//将描述符fd设置为非阻塞
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd )//添加描述符到事件表
{
    epoll_event event;
    event.data.fd = fd;//就绪事件描述符
    event.events = EPOLLIN | EPOLLET;//可读事件和ET模式(事件只触发一次)
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

void sig_handler( int sig )//信号处理函数这里将信号通过管道写端发送到主程序(统一事件源)
{
    int save_errno = errno;
    int msg = sig;
    send( pipefd[1], ( char* )&msg, 1, 0 );
    errno = save_errno;
}

void addsig( int sig )//安装信号处理函数，sig为信号
{
    struct sigaction sa;//sigaction信号结构体
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;//被信号中断的系统调用将自动重启
    sigfillset( &sa.sa_mask );//设置全部信号为进程信号掩码
    assert( sigaction( sig, &sa, NULL ) != -1 );//安装信号
}

void timer_handler()//超时处理逻辑
{
    timer_lst.tick();//调用回搏函数tick执行哪些超时的定时器
    alarm( TIMESLOT );//重置时钟
}

void cb_func( client_data* user_data )//超时连接处理逻辑
{
    epoll_ctl( epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0 );//从事件表中删除哪些已经超时的定时器
    assert( user_data );
    close( user_data->sockfd );//关闭连接
    printf( "close fd %d\n", user_data->sockfd );
}

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    int ret = 0;
    struct sockaddr_in address;//服务器端地址
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );

    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    epoll_event events[ MAX_EVENT_NUMBER ];//用于存放就绪事件
    int epollfd = epoll_create( 5 );//创建事件表
    assert( epollfd != -1 );
    addfd( epollfd, listenfd );//监听端口添加到事件表

    ret = socketpair( PF_UNIX, SOCK_STREAM, 0, pipefd );//本地双向管道用于信号处理函数将信号回传给主程序
    assert( ret != -1 );
    setnonblocking( pipefd[1] );//设置为非阻塞
    addfd( epollfd, pipefd[0] );//添加管道读端到事件表

    // add all the interesting signals here
    addsig( SIGALRM );//添加超时信号
    addsig( SIGTERM );//添加中断信号
    bool stop_server = false;//服务器是否运行

    client_data* users = new client_data[FD_LIMIT]; //分配超大用户数据数组，以空间换取时间，用于给定一个客户连接描述符作为数据下标即可快速索引到用户数据信息
    bool timeout = false;//是否超时
    alarm( TIMESLOT );//时钟开始计时

    while( !stop_server )//服务器运行逻辑
    {
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );//epoll无限期等待就绪事件
        if ( ( number < 0 ) && ( errno != EINTR ) )//若非中断导致epoll_wait出错则终止
        {
            printf( "epoll failure\n" );
            break;
        }

        for ( int i = 0; i < number; i++ )//处理就绪事件
        {
            int sockfd = events[i].data.fd;//获取就绪事件的描述符
            if( sockfd == listenfd )//若就绪描述符为监听端口则表明有新的客户连接请求
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );//允许客户连接
                addfd( epollfd, connfd );//注册新连接到事件表
                users[connfd].address = client_address;//初始化新连接的用户数据
                users[connfd].sockfd = connfd;
                util_timer* timer = new util_timer;
                timer->user_data = &users[connfd];
                timer->cb_func = cb_func;//定时器超时处理函数
                time_t cur = time( NULL );
                timer->expire = cur + 3 * TIMESLOT;//超时时间
                users[connfd].timer= timer;
                timer_lst.add_timer( timer );//添加新连接的定时器到定时器链表中去
            }
            else if( ( sockfd == pipefd[0] ) && ( events[i].events & EPOLLIN ) )//管道读端可读，说明有信号产生
            {
                int sig;
                char signals[1024];
                ret = recv( pipefd[0], signals, sizeof( signals ), 0 );//接收信号值
                if( ret == -1 )//接收出错
                {
                    // handle the error
                    continue;
                }
                else if( ret == 0 )//
                {
                    continue;
                }
                else
                {
                    for( int i = 0; i < ret; ++i )//每个信号值占1B，所以循环接收每个信号值
                    {
                        switch( signals[i] )
                        {
                            case SIGALRM://超时信号
                            {
                                timeout = true;//真的超时了
                                break;
                            }
                            case SIGTERM://中断信号，服务端该终止了
                            {
                                stop_server = true;
                            }
                        }
                    }
                }
            }
            else if(  events[i].events & EPOLLIN )//客户连接有数据发送到服务端，客户连接可读事件就绪
            {
                memset( users[sockfd].buf, '\0', BUFFER_SIZE );
                ret = recv( sockfd, users[sockfd].buf, BUFFER_SIZE-1, 0 );//接收客户数据
                printf( "get %d bytes of client data %s from %d\n", ret, users[sockfd].buf, sockfd );
                util_timer* timer = users[sockfd].timer;//获取客户连接的定时器
                if( ret < 0 )
                {
                    if( errno != EAGAIN )//非阻塞式EAGAIN不是网络错误
                    {
                        cb_func( &users[sockfd] );
                        if( timer )
                        {
                            timer_lst.del_timer( timer );//网络出错，客户连接需要关闭并从定时器链表中除名
                        }
                    }
                }
                else if( ret == 0 )//客户端连接已经关闭
                {
                    cb_func( &users[sockfd] );
                    if( timer )
                    {
                        timer_lst.del_timer( timer );//从定时器链表中除名
                    }
                }
                else//处理客户端发送来的数据
                {
                    //send( sockfd, users[sockfd].buf, BUFFER_SIZE-1, 0 );
                    if( timer )//该客户连接的定时并未超时则重置定时器超时时间
                    {
                        time_t cur = time( NULL );
                        timer->expire = cur + 3 * TIMESLOT;//新的超时时间
                        printf( "adjust timer once\n" );
                        timer_lst.adjust_timer( timer );//调整定时器链表(将重置了超时时间的定时器插入到链表中合适位置)
                    }
                }
            }
            else//其它事件逻辑，未定义
            {
                // others
            }
        }

        if( timeout )//真的超时了，该对那些超时连接动手了！更待何时？
        {
            timer_handler();//超时处理那些不听话的连接
            timeout = false;//既然处理了一波就等待下一波超时再收拾它们！
        }
    }

    close( listenfd );
    close( pipefd[1] );
    close( pipefd[0] );
    delete [] users;
    return 0;
}//总的逻辑就是:将信号与IO事件统一监听，一旦信号SIGALRM发生则调用超时处理逻辑
