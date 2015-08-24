#include "cthreadpoolmanage.h"

CthreadPoolManage::CthreadPoolManage()
{
    _minThreads = 5;    //最少保留几个线程
    _maxThreads = 5;   //最多可以有几个线程
    _waitSec = 10;           //空闲多少秒后将线程关闭
    pthread_mutex_init(&_duty_mutex, NULL);
    pthread_mutex_init(&_thread_mutex, NULL);
    checkrun = true;
}


CthreadPoolManage::CthreadPoolManage(int min, int max, int waitSec)
{
    CthreadPoolManage();
    _minThreads = min;    //最少保留几个线程
    _maxThreads = max;   //最多可以有几个线程
    _waitSec = waitSec;           //空闲多少秒后将线程关闭
}

CthreadPoolManage::~CthreadPoolManage()
{

}
void CthreadPoolManage::threadCleanUp(void* arg)
{
    threadInfo* tinfo = (threadInfo*)arg;
    tinfo->isbusy = false;
    pthread_mutex_unlock(&tinfo->mtx);
    pthread_attr_destroy (&tinfo->cThreadAttr);
    delete tinfo;
}

void* CthreadPoolManage::startThread(void* arg)
{
    cout<<"线程开始工作"<<endl;
    threadInfo* tinfo = (threadInfo*)arg;
    pthread_cleanup_push(threadCleanUp,arg);
    while(tinfo->doFlag){
        pthread_mutex_lock(&tinfo->mtx);
        if(tinfo->doit == NULL)
        {
            cout<<"开始等待任务"<<endl;
            pthread_cond_wait(&tinfo->cond,&tinfo->mtx);
            cout<<"有任务了"<<endl;
        }
        tinfo->isbusy = true;
        tinfo->doit->start(tinfo->value);
        tinfo->doit->end();
        tinfo->doit=NULL;
        tinfo->isbusy = false;
        time( &tinfo->beginTime);
        pthread_mutex_unlock(&tinfo->mtx);
    }
    //0正常执行到这儿不执行清理函数，异常会执行
    pthread_cleanup_pop(0);
    pthread_attr_destroy (&tinfo->cThreadAttr);
    delete tinfo;
    cout<<"线程结束"<<endl;
}

void CthreadPoolManage::initThread()
{
    int i = 0;
    for(i = 0;i<this->_minThreads;i++)
    {
        threadInfo *tinfo = new threadInfo;
        tinfo->doit = NULL;
        tinfo->value = NULL;
        tinfo->isbusy = false;
        tinfo->doFlag = true;
        // PTHREAD_CREATE_DETACHED  (分离线程)  和  PTHREAD _CREATE_JOINABLE  (非分离线程)
        pthread_attr_init(&tinfo->cThreadAttr);
        pthread_attr_setdetachstate(&tinfo->cThreadAttr,PTHREAD_CREATE_DETACHED );
        cout<<"初始化了一个线程"<<endl;
        if(pthread_create(&tinfo->cThreadPid,&tinfo->cThreadAttr,startThread,(void *)tinfo) != 0)
        {
            cout<<"创建线程失败"<<endl;
            break;
        }
        this->_threadList.push_back(tinfo);
    }
}

int CthreadPoolManage::addThread(std::list< CthreadPoolManage::threadInfo* >* plist, CthreadPoolManage::threadInfo* ptinfo)
{
    threadInfo *tinfo = new threadInfo;
    tinfo->doit = ptinfo->doit;
    tinfo->value = ptinfo->value;
    tinfo->isbusy = true;
    if(pthread_create(&tinfo->cThreadPid,NULL,startThread,(void *)tinfo) != 0)
    {
        cout<<"创建线程失败"<<endl;
        return -1;
    }
    plist->push_back(tinfo);
    return 0;
}


int CthreadPoolManage::putDuty(CDoit* doit, void* value)
{
    threadInfo *tinfo = new threadInfo;
    time( &tinfo->beginTime);
    tinfo->doit= doit;
    tinfo->value = value;
    pthread_mutex_lock(&_duty_mutex);
    this->_dutyList.push_back(tinfo);
    pthread_mutex_unlock(&_duty_mutex);
    return 0;
}

void* CthreadPoolManage::taskAllocation(void*arg)
{
    CthreadPoolManage * ptmanage = (CthreadPoolManage*)arg;
    int size_1 = 0;
    int size_2 = 0;
    int i_1 = 0;
    int i_2 = 0;
    bool a_1 = true;
    bool a_2 = true;
    threadInfo* ptinfo;
    threadInfo* ptinfoTmp;
    while(true){
        size_1 = 0;
        size_2 = 0;
        pthread_mutex_lock(&ptmanage->_duty_mutex);
        pthread_mutex_lock(&ptmanage->_thread_mutex);
        size_1 = ptmanage->_dutyList.size();
        size_2 =ptmanage->_threadList.size();
        for(list<threadInfo*>::iterator itorti1 = ptmanage->_dutyList.begin();itorti1 !=ptmanage->_dutyList.end();)
        {
            ptinfo = *itorti1;
            a_1 = true;
            for(list<threadInfo*>::iterator itorti2 = ptmanage->_threadList.begin();itorti2!=ptmanage->_threadList.end();itorti2++){
                ptinfoTmp = *itorti2;
                if(EBUSY == pthread_mutex_trylock(&ptinfoTmp->mtx))
                {
                    continue;
                }
                if(!ptinfoTmp->isbusy)
                {
                    ptinfoTmp->doit = ptinfo->doit;
                    ptinfoTmp->value = ptinfo->value;
                    ptinfoTmp->isbusy = true;
                    pthread_cond_signal(&ptinfoTmp->cond);
                    pthread_mutex_unlock(&ptinfoTmp->mtx);
                    a_1 = false;
                    delete ptinfo;
                    break;
                }
                pthread_mutex_unlock(&ptinfoTmp->mtx);
            }
            if(a_1){
                if(ptmanage->_threadList.size()>ptmanage->_maxThreads||ptmanage->addThread(&ptmanage->_threadList,ptinfo)!=0)
                {
                    itorti1++;
                    continue;
                }else{
                    itorti1 = ptmanage->_dutyList.erase(itorti1);
                }
                delete ptinfo;
            }else{
                itorti1 = ptmanage->_dutyList.erase(itorti1);
            }
        }
        pthread_mutex_unlock(&ptmanage->_duty_mutex);
        pthread_mutex_unlock(&ptmanage->_thread_mutex);
        usleep(USLEEP_TIME);
    }
    return 0;
}

void* CthreadPoolManage::checkThread(void* arg)
{
    CthreadPoolManage * ptmanage = (CthreadPoolManage*)arg;
    threadInfo* ptinfo;
    time_t nowtime;
    while(ptmanage->checkrun){
        sleep(CHECK_TIME);
        pthread_mutex_lock(&ptmanage->_thread_mutex);
        if(ptmanage->_threadList.size()<=ptmanage->_minThreads)
        {
            continue;
        }
        for(list<threadInfo*>::iterator itorti2 = ptmanage->_threadList.begin();itorti2!=ptmanage->_threadList.end();){
            ptinfo = *itorti2;
            if(EBUSY == pthread_mutex_trylock(&ptinfo->mtx))
            {
                itorti2++;
                continue;
            }
            time(&nowtime);
            if(ptinfo->isbusy == false && nowtime-ptinfo->beginTime>ptmanage->_waitSec)
            {
                ptinfo->doFlag = false;
                itorti2 = ptmanage->_threadList.erase(itorti2);
            }else{
                itorti2++;
            }
            pthread_mutex_unlock(&ptinfo->mtx);
        }
        pthread_mutex_unlock(&ptmanage->_thread_mutex);
    }
}

int CthreadPoolManage::start()
{
    //初始化
    this->initThread();
    //启动任务分配线程
    if(pthread_create(&tasktPid,NULL,taskAllocation,(void *)this) != 0)
    {
        cout<<"创建任务分配线程失败"<<endl;
        return -1;
    }
    //创建现程状态分配管理线程
    if(pthread_create(&checktPid,NULL,checkThread,(void *)this) != 0)
    {
        cout<<"创建线程状态分配管理线程失败"<<endl;
        return -1;
    }
    return  0;
}

///////////////////////////////
int CthreadPoolManage::getNowThreadNum()
{
    int num = 0;
    pthread_mutex_lock(&this->_thread_mutex);
    num = this->_threadList.size();
    pthread_mutex_unlock(&this->_thread_mutex);
    return num ;
}
