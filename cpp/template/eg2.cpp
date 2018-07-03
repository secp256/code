#include <iostream>

using std::cout;
using std::endl;
// 仅声明
struct Nil;

// 主模板
template <typename T>
struct IsPointer
{
    enum { Result = false };
    typedef Nil ValueType;
};

// 局部特化
template <typename T>
struct IsPointer<T*>
{
    enum { Result = true };
    typedef T ValueType;
};

int main()
{
    cout << IsPointer<int*>::Result << endl;
    cout << IsPointer<int>::Result << endl;
    IsPointer<int*>::ValueType i = 1;
    cout << i << endl;
    // IsPointer<int>::ValueType j = 1;  
    return 0;
}
