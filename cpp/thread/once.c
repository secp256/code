#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_once_t  once = PTHREAD_ONCE_INIT;

void once_run(void)
{
    printf("once run in thread=%lu\n",pthread_self());    
}

void *thread1(void *argv)
{
    printf("thread1 id = %lu\n ",pthread_self());
    sleep(1);
    pthread_once(&once,once_run);
    printf("thread1 returns\n");    
}

void *thread2(void *argv)
{
    printf("thread2 id = %lu\n",pthread_self());
    pthread_once(&once,once_run);
    printf("thread2 returns\n");    
}

int main(int argc,char** argv)
{
    pthread_t ptid1,ptid2;
    pthread_create(&ptid1,NULL,thread1,NULL);
    pthread_create(&ptid2,NULL,thread2,NULL);

    pthread_join(ptid1,NULL);
    pthread_join(ptid2,NULL);

    printf("main exit\n");

}
