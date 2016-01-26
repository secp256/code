#include <iostream> 
#include <boost/bind.hpp> 
#include <boost/function.hpp> 
using  namespace  std;
using  namespace  boost;

void  f(int  a, int  b)
{
    cout <<"Argument 1 is "  <<a <<endl;
}

void  g(int  a, int  b, int  c)
{
    cout <<"sum is "  <<a+b+c <<endl;
    cout <<"arg 1: "  <<a <<endl;
    cout <<"arg 2: "  <<b <<endl;
    cout <<"arg 3: "  <<c <<endl;
    cout <<"---------------------------"  <<endl;
}


int  main()
{
    function<void (int ,int )>  f1= bind(f, _2, _1);                 // 调整参数1，2的位置 
    f1(1 , 2 );

    function<void (int )> sum1 = bind(g, _1, _1, _1);        // 3个参数变1个 
    sum1(10 );

    function<void (int , int )> sum2 = bind(g, _2, _2, _2);       // 3个参数变2个，仅用一个 
    sum2(10 , 20 );

    function<void (int , int , int )> sum3 = bind(g, _3, _3, _3);      // 3个参数还是3个，但是仅用3个 
    sum3(10 , 20 , 30 );

    function<void (int , int , int , int )> sum4 = bind(g, _4, _4, _4);     // 3个参数变4个，但是仅用4个 
    sum4(10 , 20 , 30 , 40 );

    return  0 ;
};

