#include "Mediator.h"
int main()
{
    ConcreteMediator* mediator=new ConcreteMediator();
    //让同事认识中介者
    ColleagueA* colA=new ColleagueA(mediator);
    ColleagueB* colB=new ColleagueB(mediator);
    //让中介者认识具体的同事
    mediator->InitColleage(colA,colB);

    colA->Send("吃饭了吗？");
    colB->Send("还没吃，你请吗？");
    return 0;
}
/*
 *在面向对象系统的设计和开发过程中，对象间的交互和通信时最为常见的情况，因为对象间的交互本身就是一种通信。在系统比较小的时候，可能对象间的通信不是很多，对象也比较少，我们可以直接硬编码到各个对象的方法中。但是当系统规模变大，对象的量变引起系统复杂度的急剧增加，对象间的通信业变得越来越复杂，这时我们就要提供一个专门处理对象间交互和通信的类，这个中介者就是Mediator模式。Mediator模式提供将对象间的交互和通讯封装在一个类中，各个对象间的通信不必显示去声明和引用，大大降低了系统的复杂性能(了解一个对象总比深入熟悉n个对象要好)。另外Mediator模式还带来了系统对象间的松耦合。
 中介者模式
 中介者模式：用一个中介对象来封装一系列的对象交互。中介者使各对象不需要显式地相互引用，从而降低耦合，而且可以独立地改变它们之间的交互

 Mediator：抽象中介者，定义了同事对象之间向对方发生消息的接口
 Colleague：抽象同事，定义了中介者对象到同事对象的接口
 ConcreteMediator：具体中介者，实现抽象类的方法，它需要知道所有具体同事类，并从具体同事接收消息，向具体同事对象发出命令
 ConcreteColleague：具体同事，每个具体同事只知道自己的行为，而不了解其他同事类的情况，但它们却都认识中介者对象
 */
