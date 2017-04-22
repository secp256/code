#include <iostream>
#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace std;

void do_sum(int *values, int n) 
{ 
    int sum(0);
    for (int i = 0; i < n; ++i) { 
        sum += values[i]; 
    } 
    cout << sum << endl; 
};

class X {
    public:
        int foo(int a) {
            cout << a << endl;
            return a;
        }
};

void print(int a)
{
    cout << a << endl;
}

typedef boost::function <void(int)> SuccessPrint;

int main() 
{ 
    // norman function
    boost::function<void(int *values, int n)> sum; 
    sum = &do_sum;
    int a[] = {1,2,3,4,5}; 
    sum(a, 5);

    // class member function
    boost::function <int(X*, int)> f;
    f = &X::foo;
    X x;
    f(&x, 5);

    // example 3
    vector<SuccessPrint> print_list;
    SuccessPrint print_one = boost::bind(print, _1);
    print_list.push_back(print_one);
    SuccessPrint print_two = boost::bind(print, _1);
    print_list.push_back(print_two);
    SuccessPrint print_three = boost::bind(print, _1);
    print_list.push_back(print_three);
    
    for (int i = 0; i < print_list.size(); ++i) {
        print_list.at(i)(i);
    }
    return 0; 
}
