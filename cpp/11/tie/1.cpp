#include <iostream>

using std::cout;
using std::endl;

int test()
{
    int a, b;
    std::tie(a, b) = std::make_tuple(2, 3);
    return a + b;
}

int main()
{
    int a = test();
    cout << a << endl;
}
