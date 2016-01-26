#include <list>
#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
using namespace std;
using namespace boost;


int add1(int a, int b)
{
    return a + b;
}

class add2
{
public:
    int operator()(int lhs, int rhs)
    {
        return lhs + rhs;
    }
};

class CAdd
{
public:
    int add3(int a, int b) { return a + b; }
};


template<typename FUN, typename T>
T fun(FUN function, T lhs, T rhs)

    cout <<typeid(function).name() <<endl;
    return function(lhs, rhs);
}

int add4(int a, int b, int c)
{
    return a+b+c;
}

int fun2(boost::function< int(int,int) > function, int a, int b )
{
    cout <<typeid(function).name() <<endl;
    return function(a,b);
}


int main()
{
    boost::function< int(int,int) > padd1 = add1;
    boost::function< int(int,int) > padd2 = add2();
    boost::function< int(int,int) > padd3 = lambda::_1+lambda::_2;
    boost::function< int(int,int) > padd4 = bind(add4, 0, _1, _2);

    cout << fun2(padd1, 1, 1) <<endl;
    cout << fun2(padd2, 1, 2) <<endl;
    cout << fun2(padd3, 1, 3) <<endl;
    cout << fun2(padd4, 1, 4) <<endl;
    return 0;
}
