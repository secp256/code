#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#define MSG_FILE "/tmp/sender.c"
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
//message struct
struct msgbuf
{
    long mtype;
    char mtext[BUFFER+1];
};
int main()
{
    struct msgbuf msg;
    key_t key;
    int msgid;
    int i;
    char *myask="I'm receiver, 3 messages received from you.";
    //create a key for
    if((key=ftok(MSG_FILE,66))==-1)
    {
        fprintf(stderr,"Creat Key Error：%s \n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    //get a message queue
    if((msgid=msgget(key,PERM|IPC_CREAT))==-1)
    {
        fprintf(stderr,"Creat MessageQueue Error：%s \n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    //get a message from the queue everytime
    for(i=0; i<3; i++)
    {
        msgrcv(msgid,&msg,sizeof(struct msgbuf),1,0);
        printf("Receiver receive: %s\n",msg.mtext);
    }
    msg.mtype=2;
    //send the message that I have received the message.
    strncpy(msg.mtext,myask,BUFFER);
    msgsnd(msgid,&msg,sizeof(struct msgbuf),0);
    return 1;
}

