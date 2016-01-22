#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define PORT 9000
#define KEY 123
#define SIZE 1024


int main()
{
    char buf[100];
    memset(buf,0,100);
    int server_sockfd,client_sockfd;
    socklen_t server_len,client_len;
    struct  sockaddr_in server_sockaddr,client_sockaddr;
    /*create a socket.type is AF_INET,sock_stream*/
    server_sockfd = socket(AF_INET,SOCK_STREAM,0);
    /*the address of the sockt which is a struct include sinfamily,sin_port and sin_addr(struct)
      htons is a convert function,*/
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*the length of the sever_sockt address*/
    server_len = sizeof(server_sockaddr);
    /*
       first option is socket we create,the second option is level the left is 
       the paraments'length and value
       */
    int on;
    setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    /*bind a socket or rename a sockt*/
    if(bind(server_sockfd,(struct sockaddr*)&server_sockaddr,server_len)==-1){
        printf("bind error");
        exit(1);
    }
    /*listen the socket,the second option is 5 which is the limit number of kernal handle*/
    if(listen(server_sockfd,5)==-1){
        printf("listen error");
        exit(1);
    }

    client_len = sizeof(client_sockaddr);
    /*define a pid number*/
    pid_t ppid,pid;
    while(1) {
        int client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_sockaddr,&client_len);
        if (client_sockfd == -1) {
            printf("connect error");
            exit(1);
        }
        else {
            //send and receiv all include a socket , content and the len. 
            printf("accept success, fd: [%d]", client_sockfd);
            send(client_sockfd,"You have conected the server",strlen("You have connected the server"),0);
        }

        ppid = fork();
        if(ppid == -1) { 
            printf("fork 1 failed:"); 
        } 
        /*the subthread is used to receiver the message from client Create a process again when 
          we create father process success*/
        else if(ppid == 0){ 
            int recvbytes; 
            pid = fork(); 
            if(pid == -1){
                printf("fork 2 failed:"); 
                exit(1);
            } 
            else if(pid == 0) {
                while(1){ 
                    //set the 100 byte of the buf to 0
                    bzero(buf,100); 
                    if((recvbytes = recv(client_sockfd,buf,100,0))==-1) { 
                        perror("read client_sockfd failed:");
                    } 
                    //if the subProcess receive the message successfully
                    else if(recvbytes != 0){
                        buf[recvbytes] = '\0'; 
                        usleep(1000); 
                        printf("%s:%d said:%s\n",inet_ntoa(client_sockaddr.sin_addr), ntohs(client_sockaddr.sin_port), buf); 
                        //send same message to the client. 
                        if(send(client_sockfd,buf,recvbytes,0)==-1){ 
                            perror("send error"); 
                            break;
                        } 
                    }
                    //when send the msg successfuly end up the method 
                } 
            } 
            else if(pid>0) {

            }
        } else if(ppid>0){ //总父进程中关闭client_sockfd(因为有另一个副本在子进程中运行了)返回等待接收消息 
            close(client_sockfd); 
        } 

    } 
    return 0;
}
