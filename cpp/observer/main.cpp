#include "Observer.h"
#include <iostream>

int main()
{
    Subject* p = new ConcreateSubject();
    Observer *p1 = new ConcreateObserver(p);
    Observer *p2 = new ConcreateObserver(p);//等价于Observer *p2 = new ConcreateObserver(); p->Attach(p2); 
    p->SetState(4);
    p->Notify();

    std::cout<<"-----------------------"<<std::endl;
    p->Detach(p1);
    p->SetState(10);
    p->Notify();

    delete p;
    return 0;
}
/************************************************************************
 代码说明：在Observer模式的实现中，Subject维护一个List作为存储其所有观察者的容器。每当调用Notify操作就遍历List中Observer对象，并广播通知改变状态了(调用Observer的Update操作)。
目标的状态state可以有Subject自己改变，也可以有Observer的某个操作引起state的改变(可调用Subject的SetState操作)
当然，可以有多个Subject子类，和多个Observer的子类，也可以有多个要通知的消息。
************************************************************************/
/*
 Attach an Observer
 Attach an Observer
 SetState By ConcreateSubject
 Notify Observers's State
 GetState By ConcreateSubject
 The ObeserverState is 4
 GetState By ConcreateSubject
 The ObeserverState is 4
 -----------------------
 Detach an Observer
 SetState By ConcreateSubject
 Notify Observers's State
 GetState By ConcreateSubject
 The ObeserverState is 10
 */
