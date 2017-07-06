#include <iostream>

template<int N>
struct Fib
{
    enum { Result = Fib<N-1>::Result + Fib<N-2>::Result };
};

template<>
struct Fib<1>
{
    enum { Result = 1 };
};

template<>
struct Fib<0>
{
    enum { Result = 0 };
};

int main()
{
    int i = Fib<10>::Result;
    std::cout << i << std::endl;
    return 0;
}
