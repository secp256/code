#include <string>
#include <iostream>
using namespace std;
class Colleague;
//抽象中介者类，定义了同事对象到中介者对象的接口
class Mediator
{
public:
    virtual void Send(string msg,Colleague* col)=0;
};
//抽象同事类
class Colleague
{
protected:
    Mediator* mediator;
public:
    Colleague(Mediator* temp)
    {
        mediator=temp;
    }
};

//同事A，
//每个具体同事只知道自己的行为，而不了解其他同事类的情况，但它们却都认识中介者对象
class ColleagueA : public Colleague
{
public:
    ColleagueA(Mediator* temp) : Colleague(temp){}
    void Send(string strMsg)
    {
        mediator->Send(strMsg,this);
    }
    void Notify(string strMsg)
    {
        cout<<"同事A获得了消息："<<strMsg<<endl;
    }
};

//同事B
class ColleagueB : public Colleague
{
public:
    ColleagueB(Mediator* temp) : Colleague(temp){}
    void Send(string strMsg)
    {
        mediator->Send(strMsg,this);
    }
    void Notify(string strMsg)
    {
        cout<<"同事B获得了消息："<<strMsg<<endl;
    }
};

//具体中介者类，实现抽象类的方法
//它需要自导所有具体同事类，并从具体同事接收消息，向具体同事对象发出命令
class ConcreteMediator : public Mediator
{
private:
    ColleagueA* colA;
    ColleagueB* colB;
public:
    void InitColleage(ColleagueA* tempA,ColleagueB* tempB)
    {
        colA=tempA;
        colB=tempB;
    }
    virtual void Send(string msg,Colleague* col)
    {
        if (col==colA)
        {
            colB->Notify(msg);
        }
        else
        {
            colA->Notify(msg);
        }
    }
};
