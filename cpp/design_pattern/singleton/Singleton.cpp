#include "Singleton.h"
#include <iostream>

// 类的静态成员变量要在类体内进行声明,需要在cpp文件进行定义赋值，若在.h文件中赋值，当多个文件都保护.h文件时会出现重定义。
Singleton* Singleton::m_pStatic = NULL;//定义并初始化

Singleton* Singleton::GetInstancePtr()
{
    if (NULL == m_pStatic)
    {
        m_pStatic = new Singleton();
    }
    return m_pStatic;
}

Singleton Singleton::GetInstance()
{
    return *GetInstancePtr();
}

void Singleton::Test()
{
    std::cout << "Test!\n";
}
