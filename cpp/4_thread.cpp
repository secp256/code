#include <iostream>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t myloack = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;

int n = 0;

void *ThreadFunc(void *arg)
{
    int num = (int )arg;
    for (int i = 0; i < 20; ++i)
    {
        pthread_mutex_lock(&myloack);
        while (n != num)//1号线程第一次执行时直接跳过
            pthread_cond_wait(&mycond,&myloack);//因为多个线程共用一个条件变量，所以在条件变量被signal后，需要用while来确定是否本线程获得执行权

        if (num == 0)
            cout<<"1";
        else if(num == 1)
            cout<<"2";
        else if(num == 2)
            cout<<"3";
        else 
            cout<<"4"<<endl;
        n = (n + 1) % 4;
        // pthread_cond_signal(&mycond);
        pthread_mutex_unlock(&myloack);
        pthread_cond_broadcast(&mycond);
    }
    return (void *)0;
}

int  main(int argc, char const *argv[])
{
    pthread_t id[4];
    for (int i = 0; i < 4; ++i)
    {
        int err=pthread_create(&id[i],NULL,ThreadFunc,(void *)i);
        if (err!=0)
        {
            cout<<"create err:"<<endl;
            exit(-1);
        }

    }

    for (int i = 0; i < 4; ++i)
    {
        int ret=pthread_join(id[i],NULL);
        if (ret!=0)
        {
            cout<<"join err:"<<endl;
            exit(-1);
        }
    }
    return 0;
}
