#include <iostream>
#include <boost/make_shared.hpp> 
#include <vector>

using namespace std;
using namespace boost;

int main()  
{      
    typedef vector<shared_ptr<int> > vs;
    vs v(10);
    int i = 0;      
    for (vs::iterator pos = v.begin(); pos != v.end(); ++pos) {          
        (*pos) = make_shared<int>(++i);     //使用工厂函数赋值          
        cout << *(*pos) << ", ";            //输出值      
    }      
    cout << endl;       
    shared_ptr<int> p = v[9];      
    *p = 100;      
    cout << *v[9] << endl;  
} 
