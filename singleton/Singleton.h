#ifndef SINGLETON_H
#define SINGLETON_H
/*
下面的实现其实是singleton的一个简单实现，并不是特别通用，一般的，如果一个项目中需要使用到singleton模式比较多的话，那么一般会实现一个singleton的模板类，模板类的模板参数是需要采用singleton模式的类，比如这样实现
http://blog.csdn.net/ouyangshima/article/details/7651708
template<typename T>
class Singleton
{
//...。类的声明
};
//需要采用singleton模式的类
class Test : pulic Singleton<Test>
{
//...。类的声明
};
*/
#include <iostream>
class Singleton
{ 
public:
    // 静态成员函数,提供全局访问的接口
    static Singleton* GetInstancePtr();
    static Singleton  GetInstance();
    void Test(); 
private: 
    Singleton(){};
    ~Singleton(){};    
    static Singleton* m_pStatic;// 静态成员变量,提供全局惟一的一个实例
    class Garbo //它的唯一的工作就是在析构函数中删除Singleton的实例
    {
    public:
        ~Garbo()
        {
            if (Singleton::m_pStatic)
            {
                delete Singleton::m_pStatic; 
            }            
        }
    };//内嵌一个私有类，以防该类被在其他地方滥用
    static Garbo mGarbo;//定义一个静态成员，在程序结束的时候，系统会调用它的析构函数
};

#endif
