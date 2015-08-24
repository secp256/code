#include <pthread.h>
#include <stdio.h>

const int THREAD_NUMBER = 3;

// 子线程的互斥量和条件变量
pthread_mutex_t thread_mutex[THREAD_NUMBER];
pthread_cond_t thread_cond[THREAD_NUMBER];

// 子线程是否正在等待
bool thread_wait_flag[THREAD_NUMBER];

// 标识轮到哪个子线程输出其ID
pthread_mutex_t mutex;
int thread_turn;

void *thread_func(void *arg);

int main(int argc, char **argv)
{
    pthread_t tids[THREAD_NUMBER];

    for (int i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_mutex_init(&thread_mutex[i], NULL);
        pthread_cond_init(&thread_cond[i], NULL);
    }

    pthread_mutex_init(&mutex, NULL);
    thread_turn = 0;

    for (int i = 0; i < THREAD_NUMBER; ++i)
        thread_wait_flag[i] = false;

    for (int i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_create(&tids[i], NULL, thread_func, (void *)i);
    }

    for (int i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_join(tids[i], NULL);
    }

    printf("\n");
    return 0;
}

void *thread_func(void *arg)
{
    int id = (int)arg;
    char ch = 'A' + id;
    int count = 0;

    while (true)
    {
        if (id == thread_turn) // 若轮到当前子线程，输出ID，发送信号
        {
            printf("%c", ch);
            ++count;
            if (id == THREAD_NUMBER-1 && count == 10) // 若是第3个子线程，输出ID后，可直接退出。
                break;
            pthread_mutex_lock(&mutex);
            ++thread_turn;
            thread_turn %= THREAD_NUMBER;
            pthread_mutex_unlock(&mutex);

            while (true)
            {
                pthread_mutex_lock(&thread_mutex[thread_turn]);
                if (true == thread_wait_flag[thread_turn])
                {
                    pthread_cond_signal(&thread_cond[thread_turn]);
                    pthread_mutex_unlock(&thread_mutex[thread_turn]);
                    break;
                }
                pthread_mutex_unlock(&thread_mutex[thread_turn]);
            }
            if (count == 10) // 若是第1、2个子线程，发出信号后，退出
                break;
        }
        else // 否则，等待
        {
            pthread_mutex_lock(&thread_mutex[id]);
            thread_wait_flag[id] = true;
            pthread_cond_wait(&thread_cond[id], &thread_mutex[id]);
            thread_wait_flag[id] = false;
            pthread_mutex_unlock(&thread_mutex[id]);
        }
    }
    return (void *)0;
}

