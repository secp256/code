#include "Observer.h"
#include <iostream>
#include <algorithm>

/* --------------------------------------------------------------------
|   Subject类成员函数的实现 
 ----------------------------------------------------------------------*/
void Subject::Attach(Observer *pObserver)
{
    std::cout << "Attach an Observer\n";
    m_ListObserver.push_back(pObserver);
}

void Subject::Detach(Observer *pObserver)
{
    std::list<Observer*>::iterator iter;
    iter = std::find(m_ListObserver.begin(), m_ListObserver.end(), pObserver);

    if (m_ListObserver.end() != iter)
    {
        m_ListObserver.erase(iter);
    }
    std::cout << "Detach an Observer\n";
}

void Subject::Notify()
{
    std::cout << "Notify Observers's State\n";
    std::list<Observer*>::iterator iter1, iter2;
    for (iter1 = m_ListObserver.begin(), iter2 = m_ListObserver.end(); iter1 != iter2;++iter1)
    {
        (*iter1)->Update(this);
    }
} 

Subject::~Subject()
{
    std::list<Observer*>::iterator iter1, iter2, temp;
    for (iter1 = m_ListObserver.begin(), iter2 = m_ListObserver.end();iter1 != iter2;)
    {
        temp = iter1;
        ++iter1;
        delete (*temp);
    }
    m_ListObserver.clear();
}

/* --------------------------------------------------------------------
|   ConcreateSubject类成员函数的实现 
----------------------------------------------------------------------*/
void ConcreateSubject::SetState(STATE nState)
{
    std::cout << "SetState By ConcreateSubject\n";
    m_nSubjectState = nState;
    //Notify();也可以写在内部，这样就不用每次调用SetState()后在调用Notify()
}

STATE ConcreateSubject::GetState()
{
    std::cout << "GetState By ConcreateSubject\n";
    return m_nSubjectState;
}

/* --------------------------------------------------------------------
|   ConcreateObserver类成员函数的实现 
----------------------------------------------------------------------*/ 
ConcreateObserver::ConcreateObserver(Subject* sub) : Observer()
{
    sub->Attach(this);
}
void ConcreateObserver::Update(Subject* pSubject)
{
    if (NULL == pSubject)
        return;
    m_nObserverState = pSubject->GetState(); 
    std::cout << "The ObeserverState is " << m_nObserverState << std::endl;
}
