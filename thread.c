#include <pthread.h>
#include <stdio.h>

// 互斥锁，条件变量
pthread_mutex_t mutex;
pthread_cond_t cond;

// 循环次数
int main_count = 0;
int subthread_count = 0;

// 线程等待标志
bool main_thread_wait_flag = false;
bool subthread_wait_flag = false;

void main_thread_func();
void *subthread_func(void *arg);

int main(int argc, char **argv)
{
    pthread_t tid;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&tid, NULL, subthread_func, NULL);
    main_thread_func();

    pthread_join(tid, NULL);

    return 0;
}

void main_thread_func()
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        main_thread_wait_flag = true;
        pthread_cond_wait(&cond, &mutex);
        main_thread_wait_flag = false;
        pthread_mutex_unlock(&mutex);

        for (int i = 1; i <= 100; ++i)
        {
            fprintf(stdout, "main thread: %d\n", i);
        }


        while (true)
        {
            pthread_mutex_lock(&mutex);
            if (true == subthread_wait_flag)
            {
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
        }

        ++main_count;
        if (main_count >= 50)
        {
            fprintf(stdout, "main thread loop 50 times\n");
            break;
        }
    }
}

void *subthread_func(void *arg)
{
    while (true)
    {
        for (int i = 1; i <= 10; ++i)
        {
            fprintf(stdout, "subthread: %d\n", i);
        }

        while (true)
        {
            pthread_mutex_lock(&mutex);
            if (true == main_thread_wait_flag)
            {
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_lock(&mutex);
        subthread_wait_flag = true;
        pthread_cond_wait(&cond, &mutex);
        subthread_wait_flag = false;
        pthread_mutex_unlock(&mutex);

        ++subthread_count;
        if (subthread_count >= 50)
        {
            fprintf(stdout, "subthread loop 50 times\n");
            break;
        }
    }
    return (void *)0;
}
