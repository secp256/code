#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_t thread;
pthread_cond_t cond;
pthread_mutex_t mutex;
unsigned char flag = 1;

void * thr_fn(void * arg)
{
    struct timeval now;
    struct timespec outtime;
    pthread_mutex_lock(&mutex);
    while (flag)
    {
        printf("thread sleep now\n");
        gettimeofday(&now, NULL);
        outtime.tv_sec = now.tv_sec + 10;
        outtime.tv_nsec = now.tv_usec * 1000;
        pthread_cond_timedwait(&cond, &mutex, &outtime);
    }
    pthread_mutex_unlock(&mutex);
    printf("thread exit\n");
}

int main()
{
    char c ;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    if (0 != pthread_create(&thread, NULL, thr_fn, NULL))
    {
        printf("error when create pthread,%d\n", errno);
        return 1;
    }
    
    while ((c = getchar()) != 'q');
    printf("Now terminate the thread!\n");
    flag = 0;
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    printf("Wait for thread to exit\n");
    pthread_join(thread, NULL);
    printf("Bye\n");
    return 0;
}
