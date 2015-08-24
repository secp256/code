#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<assert.h>
using namespace std;
class test{
    public:
        test(){
            a = new int(10);
        }
        void show(){
            sleep(1);//访问成员函数休眠1s是为了让析构进行到一半
            cout<<"show()::"<<*a<<endl;
        }
        ~test(){
            cout << "a: " << *a << endl;
            delete a;//析构先将a删除
            sleep(2);//然后休眠2s是为了造成正在析构的逻辑
            cout<<"deconstruct()"<<endl;
        }
    private:
        int *a;
};
void* worker1(void* arg){
    test* temp=(test*)arg;
    temp->~test();//线程1析构对象
}
void* worker2(void* arg){
    test* temp=(test*)arg;
    temp->show();//线程2访问对象的成员啊，但是线程1正好将a删除但是析构正在进行...
}
int main(){
    pthread_t ptd[2];
    test *one=new test;
    int ret=pthread_create(&ptd[0],NULL,worker1,one);
    assert(ret==0);
    ret=pthread_create(&ptd[1],NULL,worker2,one);
    pthread_join(ptd[0],NULL);
    pthread_join(ptd[1],NULL);
    return 0;
}
