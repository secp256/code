//信号量---线程间通信
//“生产者消费者” 问题
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#define msleep(x)   usleep(x*1000)
#define PRODUCT_SPEED       3               //生产速度
#define CONSUM_SPEED        1               //消费速度
#define INIT_NUM                3               //仓库原有产品数
#define TOTAL_NUM               10          //仓库容量

sem_t p_sem, c_sem, sh_sem;
int num=INIT_NUM;

void product(void)                          //生产产品
{
    sleep(PRODUCT_SPEED);
}

int add_to_lib()                                //添加产品到仓库
{
    num++;//仓库中的产品增加一个
    msleep(500);
    return num;
}

void consum()                                       //消费
{
    sleep(CONSUM_SPEED);
}

int sub_from_lib()                          //从仓库中取出产品
{
    num--; //仓库中的产品数量减一
    msleep(500);
    return num;
}

void *productor(void *arg)          //生产者线程
{
    while(1)
    {
        sem_wait(&p_sem);//生产信号量减一
        product();// 生产延时       
        sem_wait(&sh_sem);//这个信号量是用来互斥的
        printf("push into! tatol_num=%d\n",add_to_lib());
        sem_post(&sh_sem);  
        sem_post(&c_sem);  //消费信号量加一
    }
}

void *consumer(void *arg)               //消费者线程
{
    while(1)
    {

        sem_wait(&c_sem); //消费者信号量减一    
        sem_wait(&sh_sem);
        printf("pop out! tatol_num=%d\n",sub_from_lib());
        sem_post(&sh_sem);      
        sem_post(&p_sem);//生产者信号量加一 
        consum();//消费延时



    }
}

int main()
{
    pthread_t tid1,tid2;
    sem_init(&p_sem,0,TOTAL_NUM-INIT_NUM);

    sem_init(&c_sem,0,INIT_NUM);

    sem_init(&sh_sem,0,1);

    pthread_create(&tid1,NULL,productor,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}
