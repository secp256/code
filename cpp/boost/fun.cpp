#include <iostream>
#include <boost/function.hpp>

using namespace std;
using namespace boost;

int func1(int i) { return (i*5); }
bool func2(int i, double d) { return (i > d); }

int main()
{
    function1<int, int> f1; //(1)
    f1 = &func1;
    cout << f1(10) << endl; //50

    function2<bool, int, double> f2; //(2)
    f2 = &func2;
    cout << f2(10, 1.1) << endl; //1

    return 0;
}
