#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define PRODUCER_NUM   5  //生产者数
#define CONSUMER_NUM   3  //消费者数
#define PRD_NUM        20 //最多生产的产品数
#define DELAY_TIME     3  //生产(或消费)任务之间的最大时间间隔
#define QUEUE_SIZE     (PRD_NUM+1) //队列最大容纳QUEUE_SIZE-1个元素

typedef struct{
    int aData[QUEUE_SIZE];  //队列元素
    int dwHead;  //指向队首元素
    int dwTail;  //指向队尾元素的下一个元素
}T_QUEUE, *PT_QUEUE;

// #define QUEUE_SIZE   5 //队列最大容纳QUEUE_SIZE-1个元素

T_QUEUE gtQueue;
pthread_mutex_t gtQueLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gtPrdCond = PTHREAD_COND_INITIALIZER; //Full->Not Full
pthread_cond_t gtCsmCond = PTHREAD_COND_INITIALIZER; //Empty->Not Empty

//初始化循环队列
void InitQue(PT_QUEUE ptQue)
{
    memset(ptQue, 0, sizeof(*ptQue));
}

//向循环队列中插入元素
void EnterQue(PT_QUEUE ptQue, int dwElem)
{
    if(IsQueFull(ptQue))
    {
        printf("Elem %d cannot enter Queue %p(Full)!\n", dwElem, ptQue);
        return;
    }
    ptQue->aData[ptQue->dwTail]= dwElem;
    ptQue->dwTail = (ptQue->dwTail + 1) % QUEUE_SIZE;
}

//从循环队列中取出元素
int LeaveQue(PT_QUEUE ptQue)
{
    if(IsQueEmpty(ptQue))
    {
        printf("Queue %p is Empty!\n", ptQue);
        return -1;
    }
    int dwElem = ptQue->aData[ptQue->dwHead];
    ptQue->dwHead = (ptQue->dwHead + 1) % QUEUE_SIZE;
    return dwElem;
}

//从队首至队尾依次显示队中元素值
void DisplayQue(PT_QUEUE ptQue)
{
    if(IsQueEmpty(ptQue))
    {
        printf("Queue %p is Empty!\n", ptQue);
        return;
    }

    printf("Queue Element: ");
    int dwIdx = ptQue->dwHead;
    while((dwIdx % QUEUE_SIZE) != ptQue->dwTail)
        printf("%d ", ptQue->aData[(dwIdx++) % QUEUE_SIZE]);

    printf("\n");
}

//判断循环队列是否为空
int IsQueEmpty(PT_QUEUE ptQue)
{
    return ptQue->dwHead == ptQue->dwTail;
}

//判断循环队列是否为满
int IsQueFull(PT_QUEUE ptQue)
{
    return (ptQue->dwTail + 1) % QUEUE_SIZE == ptQue->dwHead;
}

//获取循环队列元素数目
int QueDataNum(PT_QUEUE ptQue)
{
    return (ptQue->dwTail - ptQue->dwHead + QUEUE_SIZE) % QUEUE_SIZE;
}

//获取循环队列队首位置
int GetQueHead(PT_QUEUE ptQue)
{
    return ptQue->dwHead;
}
//获取循环队列队首元素
int GetQueHeadData(PT_QUEUE ptQue)
{
    return ptQue->aData[ptQue->dwHead];
}
//获取循环队列队尾位置
int GetQueTail(PT_QUEUE ptQue)
{
    return ptQue->dwTail;
}

/*
static T_QUEUE gtQueue;

void QueueTest(void)
{
    InitQue(&gtQueue);
    printf("Enter Queue 1,2,3,4,5...\n");
    EnterQue(&gtQueue, 1);
    EnterQue(&gtQueue, 2);
    EnterQue(&gtQueue, 3);
    EnterQue(&gtQueue, 4);
    EnterQue(&gtQueue, 5);
    printf("Queue Status: Empty(%d), Full(%d)\n", IsQueEmpty(&gtQueue), IsQueFull(&gtQueue));
    printf("Queue Elem Num: %u\n", QueDataNum(&gtQueue));
    printf("Head: %u(%d)\n", GetQueHead(&gtQueue), GetQueHeadData(&gtQueue));
    printf("Tail: %u\n", GetQueTail(&gtQueue));
    DisplayQue(&gtQueue);

    printf("\nLeave Queue...\n");
    printf("Leave %d\n", LeaveQue(&gtQueue));
    printf("Leave %d\n", LeaveQue(&gtQueue));
    printf("Leave %d\n", LeaveQue(&gtQueue));
    DisplayQue(&gtQueue);

    printf("\nEnter Queue 6,7...\n");
    EnterQue(&gtQueue, 6);
    EnterQue(&gtQueue, 7);
    DisplayQue(&gtQueue);

    printf("\nLeave Queue...\n");
    printf("Leave %d\n", LeaveQue(&gtQueue));
    printf("Leave %d\n", LeaveQue(&gtQueue));
    printf("Leave %d\n", LeaveQue(&gtQueue));
    DisplayQue(&gtQueue);
}

int main()
{
    QueueTest();
    return 0;
}
*/

void *ProducerThread(void *pvArg)
{
    pthread_detach(pthread_self());
    int dwThrdNo = (int)pvArg;
    while(1)
    {
        pthread_mutex_lock(&gtQueLock);
        while(IsQueFull(&gtQueue))  //队列由满变为非满时，生产者线程被唤醒
            pthread_cond_wait(&gtPrdCond, &gtQueLock);

        EnterQue(&gtQueue, GetQueTail(&gtQueue)); //将队列元素下标作为元素值入队
        if(QueDataNum(&gtQueue) == 1) //当生产者开始产出后，通知(唤醒)消费者线程
            pthread_cond_broadcast(&gtCsmCond);
        printf("[Producer %2u]Current Product Num: %u\n", dwThrdNo, QueDataNum(&gtQueue));

        pthread_mutex_unlock(&gtQueLock);
        sleep(rand()%DELAY_TIME + 1);
    }
}

void *ConsumerThread(void *pvArg)
{
    pthread_detach(pthread_self());
    int dwThrdNo = (int)pvArg;
    while(1)
    {
        pthread_mutex_lock(&gtQueLock);
        while(IsQueEmpty(&gtQueue)) //队列由空变为非空时，消费者线程将被唤醒
            pthread_cond_wait(&gtCsmCond, &gtQueLock);

        if(GetQueHead(&gtQueue) != GetQueHeadData(&gtQueue))
        {
            printf("[Consumer %2u]Product: %d, Expect: %d\n", dwThrdNo,
                   GetQueHead(&gtQueue), GetQueHeadData(&gtQueue));
            exit(0);
        }  
        LeaveQue(&gtQueue);
        if(QueDataNum(&gtQueue) == (PRD_NUM-1)) //当队列由满变为非满时，通知(唤醒)生产者线程
            pthread_cond_broadcast(&gtPrdCond);
        printf("[Consumer %2u]Current Product Num: %u\n", dwThrdNo, QueDataNum(&gtQueue));

        pthread_mutex_unlock(&gtQueLock);
        sleep(rand()%DELAY_TIME + 1);
    }
}

int main(void)
{
    InitQue(&gtQueue);
    srand(getpid());  //初始化随机函数发生器

    pthread_t aThrd[CONSUMER_NUM+PRODUCER_NUM];
    int dwThrdIdx;
    for(dwThrdIdx = 0; dwThrdIdx < CONSUMER_NUM; dwThrdIdx++)
    {  //创建CONSUMER_NUM个消费者线程，传入(void*)dwThrdIdx作为ConsumerThread的参数
        pthread_create(&aThrd[dwThrdIdx], NULL, ConsumerThread, (void*)dwThrdIdx);
    }
    sleep(2);
    for(dwThrdIdx = 0; dwThrdIdx < PRODUCER_NUM; dwThrdIdx++)
    {
        pthread_create(&aThrd[dwThrdIdx+CONSUMER_NUM], NULL, ProducerThread, (void*)dwThrdIdx);
    }

    while(1);
    return 0 ;
}


