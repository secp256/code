#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>
typedef int STATE;
class Observer;
// Subject抽象基类,只需要知道Observer基类的声明就可以了，可以译为主题或抽象通知者。
//实现观察者模式的形式其实就是"注册——通知——注销"
class Subject
{
public:
    Subject() : m_nSubjectState(-1){}
    virtual ~Subject();

    void Notify();                          // 通知所有观察者对象
    void Attach(Observer *pObserver);       // 新增一个观察者对象
    void Detach(Observer *pObserver);       // 删除一个观察者对象

    // 虚函数,提供默认的实现,派生类可以自己实现来覆盖基类的实现
    virtual void    SetState(STATE nState)=0;   // 设置状态
    virtual STATE   GetState()=0;       // 得到状态

protected:
    STATE m_nSubjectState;                  // 模拟保存Subject状态的变量
    std::list<Observer*>    m_ListObserver; // 保存Observer指针的链表
};
 
// ConcreateSubject类,派生在Subject类，叫做具体主题或具体通知者
class ConcreateSubject  : public Subject
{
public:
    ConcreateSubject() : Subject(){}
    virtual ~ConcreateSubject(){}

    // 派生类自己实现来覆盖基类的实现,是成对出现的，由Subject来设置状态，由Observer来得到改变的状态
    virtual void    SetState(STATE nState); // 设置状态
    virtual STATE   GetState();     // 得到状态
};

//Observe类，抽象观察者，为所有的具体观察者定义一个接口，在得到主题通知时更新自己。这个接口叫做更新接口。更新接口通常包含一个Update()方法，这个方法叫做更新方法
class Observer
{
public:
    Observer() : m_nObserverState(-1){}
    virtual ~Observer(){}

    // 纯虚函数,各个派生类可能有不同的实现
    // 通知Observer状态发生了变化
    virtual void Update(Subject* pSubject) = 0;
protected:
    STATE m_nObserverState;                 // 模拟保存Observer状态的变量
};

// ConcreateObserver类派生自Observer
class ConcreateObserver : public Observer 
{
public: 
    ConcreateObserver():Observer(){}
    ConcreateObserver(Subject* sub);//使用其构造函数，省去了在其外部调用sub->Attach
    virtual ~ConcreateObserver(){}

    // 虚函数,实现基类提供的接口
    virtual void Update(Subject* pSubject);
};

#endif
