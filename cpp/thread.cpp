#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* fun(void* v)
{
    sleep(10);
    printf("grandson thread\n");
}

void* thrd_start_routine(void* v)
{
    pthread_t thd;
    pthread_create(&thd, NULL, &fun, NULL);
    sleep(5);
    printf("created thread\n");
}

int main()
{
    pthread_t thrdid;

    pthread_create(&thrdid, NULL, &thrd_start_routine, NULL);

    sleep(15);
    printf("main thread\n");
    return  0;
}
