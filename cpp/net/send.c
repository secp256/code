#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MSG_FILE "/tmp/sender.c"
#define BUFFER_SIZE 255
#define PERM S_IRUSR|S_IWUSR

struct msgbuf
{
    long mtype;
    char mtext[BUFFER_SIZE+1];
};

//create three messages
char *message[3]={"I'm sender,there are some message for you.","Message1",
"Message2"};

int main()
{
    struct  msgbuf msg;
    key_t key;
    int msgid;
    if((key=ftok(MSG_FILE,66))==-1)
    {
        printf("error in creating key\n");
        exit(0);
    }
    if((msgid=msgget(key,PERM|IPC_CREAT))==-1)
    {
        printf("error in creating get message\n");
        exit(0);
    }
//set the type of the message
    msg.mtype=1;
    int i;
//send the message
    for(i=0; i<3; i++)
    {
        strncpy(msg.mtext,message[i],BUFFER_SIZE);
        msgsnd(msgid,&msg,sizeof(struct msgbuf),0);
    }
    memset(&msg,'\0',sizeof(struct msgbuf));
/*receive the message,the third arguments show the type of message will be received
*/
    msgrcv(msgid,&msg,sizeof(struct msgbuf),2,0);
    printf("%s\n",msg.mtext);
//delete the message queue.
    if(msgctl(msgid,IPC_RMID,0)==-1)
    {
        printf("error in deleting msg queue\n");
        exit(0);
    }
    return 1;
}
