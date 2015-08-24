#include <deque>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

using namespace std;

template<class DataType>
class Message_Queue
{
public:
    //构造函数，初始化2个互斥锁和1个条件变量
    Message_Queue():_nready(0)
    {
        pthread_mutex_init(&_mutex, NULL);
        pthread_mutex_init(&_ready_mutex, NULL);
        pthread_cond_init(&_cond, NULL);
    }

    //推消息
    int push_msg(DataType &d)
    {
        //加锁_mutex，向queue里推消息
        pthread_mutex_lock(&_mutex); 
        _queue.push_back(d);
        pthread_mutex_unlock(&_mutex);

        //加锁_ready_mutex，判断是否需要唤醒挂起的消费者
        pthread_mutex_lock(&_ready_mutex);
        if (!_nready)
            pthread_cond_signal(&_cond); //唤醒阻塞在此消息队列上的消费者

        _nready++; //计数器++
        pthread_mutex_unlock(&_ready_mutex);

        return 0;        
    }

    //取消息
    int get_msg(DataType &d)
    {
        //加锁，查看计数器，看是否需要挂起
        pthread_mutex_lock(&_ready_mutex);
        while (_nready == 0) //计数器为0，队列为空，挂起
            pthread_cond_wait(&_cond, &_ready_mutex); //为空时，所有消费者会阻塞在此

        //当被生产者唤醒时，消费者重新加_ready_mutex锁，_nready > 0, 程序跳出while(_nready)循环继续运行
        _nready--;
        pthread_mutex_unlock(&_ready_mutex);

        //加锁，取队列操作
        pthread_mutex_lock(&_mutex); 
        d = _queue.front();
        _queue.pop_front();
        pthread_mutex_unlock(&_mutex);

        return 0;
    }

private:
    pthread_cond_t     _cond;
    int                 _nready;
    pthread_mutex_t     _ready_mutex;
    pthread_mutex_t     _mutex;
    deque<DataType> _queue;
};


//测试程序，模拟N个生产者和M个消费者使用消息队列同时工作的情况。
void *consume(void *arg) //消费者线程
{
    Message_Queue<int> *queue = (Message_Queue<int> *)arg;
    int i;
    for (; ; )
    {
        sleep(1);
        printf("[%lu] ready to get_msg\n", pthread_self());
        queue->get_msg(i);
        printf("[%lu] get msg = %d\n", pthread_self(), i);
        i++;
    }

}

void *produce(void *arg) //生产者线程
{
    Message_Queue<int> *queue = (Message_Queue<int> *)arg;
    int i;
    for (; ; )
    {
        sleep(1);
        printf("[%lu] ready to push_msg\n", pthread_self());
        queue->push_msg(i);
        printf("[%lu] push_msg finished\n", pthread_self());
        i++;
    }
}

const int c = 2;
const int p = 3;

int main()
{

    Message_Queue<int> msg_queue;

    int i;
    pthread_t pid_c[c];
    pthread_t pid_p[p];
    printf("create %d consume................\n", c);
    for (i = 0; i < c; i++)
    {
        pthread_create(&pid_c[i], NULL, consume, &msg_queue);
    }

    printf("create %d produce.............\n", p);
    for (i = 0; i < p; i++)
    {
        pthread_create(&pid_p[i], NULL, produce, &msg_queue);
    }

    for (i = 0; i < c; i++)
        pthread_join(pid_c[i], NULL);

    for (i = 0; i < p; i++)
        pthread_join(pid_p[i], NULL);
}
