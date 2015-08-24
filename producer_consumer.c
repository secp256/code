// producer_consumer.cpp
//////////////////////////////////////////////////////////////////////
// 有一个生产者在生产产品，这些产品将提供给若干个消费者去消费，为了使生产者和消费者能并发执行，
// 在两者之间设置一个有多个缓冲区的缓冲池，生产者将它生产的产品放入一个缓冲区中，消费者可以从缓
// 冲区中取走产品进行消费，所有生产者和消费者都是异步方式运行的，但它们必须保持同步，即不允许消
// 费者到一个空的缓冲区中取产品，也不允许生产者向一个已经装满产品且尚未被取走的缓冲区中投放产品。
//////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


const int BUFFER_LENGTH = 100;
int buffer[BUFFER_LENGTH];
int front = 0, rear = -1; // 缓冲区的前端和尾端
int size = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;

bool producer_wait = false;
bool consumer_wait = false;

void *producer(void *arg);
void *consumer(void *arg);

int main(int argc, char **argv)
{
    pthread_t producer_id;
    pthread_t consumer_id;

    pthread_create(&producer_id, NULL, producer, NULL);

    pthread_create(&consumer_id, NULL, consumer, NULL);

    sleep(1);

    return 0;
}

void *producer(void *arg)
{
    pthread_detach(pthread_self());

    while (true)
    {
        pthread_mutex_lock(&mutex);
        if (size == BUFFER_LENGTH) // 如果缓冲区已满，等待; 否则，添加新产品
        {
            printf("buffer is full. producer is waiting...\n");
            producer_wait = true;
            pthread_cond_wait(&full_cond, &mutex);
            producer_wait = false;
        }
        // 往尾端添加一个产品
        rear = (rear + 1) % BUFFER_LENGTH;
        buffer[rear] = rand() % BUFFER_LENGTH;
        printf("producer produces the item %d: %d\n", rear, buffer[rear]);
        ++size;
        if (size == 1) // 如果当前size=1, 说明以前size=0, 消费者在等待，则给消费者发信号
        {
            while (true)
            {
                if (consumer_wait)
                {
                    pthread_cond_signal(&empty_cond);
                    break;
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer(void *arg)
{
    pthread_detach(pthread_self());

    while (true)
    {
        pthread_mutex_lock(&mutex);
        if (size == 0) // 如果缓冲区已空，等待; 否则，消费产品
        {
            printf("buffer is empty. consumer is waiting...\n");
            consumer_wait = true;
            pthread_cond_wait(&empty_cond, &mutex);
            consumer_wait = false;
        }
        // 从前端消费一个产品
        printf("consumer consumes an item%d: %d\n", front, buffer[front]);
        front = (front + 1) % BUFFER_LENGTH;
        --size;
        if (size == BUFFER_LENGTH-1) // 如果当前size=BUFFER_LENGTH-1，说明以前生产者在等待，则给生产者发信号
        {
            while (true)
            {
                if (producer_wait)
                {
                    pthread_cond_signal(&full_cond);
                    break;
                }
            }
        }
        pthread_mutex_unlock(&mutex);
    }
}

