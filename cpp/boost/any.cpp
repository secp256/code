#include <iostream>
#include <vector>
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

    /*
    std::vector<boost::any> v;
    v.push_back(1);
    v.push_back(1.414);
    v.push_back(std::string("hello world"));
    for (auto i : v) {
        cout << i << endl;
    }
    */

    return 0;
}
