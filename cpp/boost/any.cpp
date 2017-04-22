#include <iostream>
#include <boost/any.hpp>

using namespace std;
int main()
{
    boost::any a(100);
    cout << boost::any_cast<int>(a) << " : " << a.type().name() <<endl; 

    boost::any b(string("hello"));
    cout << boost::any_cast<string>(b) << " : " << b.type().name() <<endl; 

    b = a;
    cout << boost::any_cast<int>(a) << " : " << b.type().name() <<endl; 

    return 0;
}
