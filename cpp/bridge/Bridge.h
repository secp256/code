#ifndef BRIDEG_H
#define BRIDEG_H

class Implementor;

// 维护一个Implementor类的指针
class Abstraction
{
public:
    Abstraction(Implementor* pImplementor);
    virtual ~Abstraction();

    void Operation();
protected:
    Implementor* m_pImplementor;
};

// 为实现Abstraction定义的抽象基类,定义了实现的接口函数
class Implementor
{
public:
    Implementor(){}
    virtual ~Implementor(){}

    virtual void OperationImpl() = 0;
};

// 继承自Implementor,是Implementor的不同实现之一
class ConcreateImplementorA : public Implementor
{
public:
    ConcreateImplementorA(){}
    virtual ~ConcreateImplementorA(){}

    virtual void OperationImpl();
};

// 继承自Implementor,是Implementor的不同实现之一
class ConcreateImplementorB : public Implementor
{
public:
    ConcreateImplementorB(){}
    virtual ~ConcreateImplementorB(){}

    virtual void OperationImpl();
};

#endif
