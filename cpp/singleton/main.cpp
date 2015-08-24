#include "Singleton.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
int main()
{
    //Singleton* s2=&(Singleton::GetInstance());//无法访问private或protected成员
    // 不用初始化类对象就可以访问了
    Singleton* s1=Singleton::GetInstancePtr();
    s1->Test();
    Singleton* s2=Singleton::GetInstancePtr();
    if (s1==s2)
        std::cout<<"ok"<<std::endl;
    else
        std::cout<<"no"<<std::endl; 
    return 0;
}
