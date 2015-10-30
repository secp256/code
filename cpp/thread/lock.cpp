#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>

using namespace std;

class test{//持有一个互斥锁
    public:
        test(){
            pthread_mutex_init(&mutex,NULL);
        }
        void lock(){
            pthread_mutex_lock(&mutex);
        }
        void unlock(){
            pthread_mutex_unlock(&mutex);
        }
    private:
        pthread_mutex_t mutex;
};

test a,b;

void swap(test& a,test& b){//
    a.lock();//先锁住a
    sleep(2);//睡眠是为了另一个线程先锁住b，造成两个线程死锁
    b.lock();//再锁b
    b.unlock();
    a.unlock();
}
void _swap(test& a,test& b){
    if(&a<&b){
        swap(a,b);
    }
    else{
        swap(b,a);
    }
}
void* worker1(void* arg){//线程1先锁a再锁b
    swap(a,b);
    //_swap(a,b);//解决方法是请求锁的时候先比较对象地址
    return NULL;
}
void* worker2(void* arg){//线程2先锁b再锁a，因此两个线程死锁
    swap(b,a);
    //_swap(b,a);//解决方法
    return NULL;
}

int main() {
    pthread_t pid1,pid2;
    int ret=pthread_create(&pid1,NULL,worker1,NULL);
    assert(ret==0);

    ret=pthread_create(&pid2,NULL,worker2,NULL);
    assert(ret==0);

    ret=pthread_join(pid1,NULL);
    assert(ret==0);

    ret=pthread_join(pid2,NULL);
    assert(ret==0);

    return 0;
}
