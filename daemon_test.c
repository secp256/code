#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_daemon(void)
{
    int pid;
    int i;
    if(pid=fork())
        // 结束父进程
        exit(0);
    else if(pid< 0)
        exit(1);

    sleep(10);

    //子进程，后台继续执行
    setsid();
    //并与控制终端分离
    //之前parent和child运行在同一个session里,而且parent是session头,所以,
    //所以作为session头的parent如果exit结束执行的话,那么子进程会成为孤儿进程，并被init收养
    //执行setsid()之后, child将重新获得一个新的会话session组id
    //这时parent退出之后,将不会影响到child了

    if(pid=fork())
        exit(0);//是第一子进程，结束第一子进程
    else if(pid< 0)
        exit(1);//fork失败，退出
    //是第二子进程，继续
    //第二子进程不再是会话组长
    for(i=0;i< NOFILE;++i)
        //关闭打开的文件描述符
        close(i);
    chdir("/tmp"); //改变工作目录到/tmp
    umask(0);//重设 文件创建掩模
    return;
}

int main()
{
    FILE *fp;time_t t;
    init_daemon();//初始化为Daemon
    while(1)//每隔一分钟向test.log报告运行状态
    {
        sleep(5);//睡 眠一分钟
        if ((fp=fopen("/home/zhuliting/test/test.log","a")) >= 0)
        {
            t = time(0);
            fprintf(fp,"Im here at %s", asctime(localtime(&t)));
            fclose(fp);
        }
    }
    return 0;
}
