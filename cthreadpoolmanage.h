#ifndef CTHREADPOOLMANAGE_H
#define CTHREADPOOLMANAGE_H
#include <iostream>
#include <pthread.h>
#include <unistd.h> 
#include <list>
#include <vector>
#include <time.h>
#include <asm/errno.h>

#define USLEEP_TIME 100
#define CHECK_TIME 1

using namespace std;

class CDoit
{
public:
    virtual int start(void *){};
    virtual int end(){};
};


class CthreadPoolManage
{
private:
    int _minThreads;    //最少保留几个线程
    int _maxThreads;   //最多可以有几个线程
    int _waitSec;           //空闲多少秒后将线程关闭
    class threadInfo{
    public:
        threadInfo(){
            isbusy = false; 
            doFlag = true;
        }
        //
        pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
        bool  isbusy;      //是否空闲
        bool  doFlag;
        //
        time_t beginTime;         //线程不工作开始时间
        pthread_t cThreadPid;  //线程id
        pthread_attr_t cThreadAttr; //线程属性
        CDoit * doit;                  //任务类
        void   * value;                //需要传递的值
    };
    //线程函数
    static void* startThread(void*);
    //任务队列锁
    pthread_mutex_t  _duty_mutex;
    //任务队列
    list<threadInfo*> _dutyList;
    //线程队列锁
    pthread_mutex_t  _thread_mutex;
    //线程队列
    list<threadInfo*> _threadList;

    ///初始化，创建最小个数线程///
    void initThread();  
    ///任务分配线程///
    static void* taskAllocation(void*arg);
    pthread_t tasktPid;
    ///线程销毁、状态检查线程///
    static void* checkThread(void* arg);
    pthread_t checktPid;
    bool   checkrun;

    //线程异常退出清理
    static void threadCleanUp(void* arg);

    //
    int addThread(list<threadInfo*> *plist,threadInfo* ptinfo);

public:
    CthreadPoolManage();
    /*
       保留的最少线程，最多线程数，空闲多久销毁，保留几个线程的冗余
       */
    CthreadPoolManage(int min,int max,int waitSec);
    ~CthreadPoolManage();

    int start();
    //任务注入器
    int putDuty(CDoit *,void *);

    int getNowThreadNum();

};

#endif // CTHREADPOOLMANAGE_H
