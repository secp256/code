#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace boost;
using namespace std;

// g++ weak_ptr.cpp -lboost_system
int main()
{
    shared_ptr<int> sp(new int(10));
    assert(sp.use_count() == 1);
    cout << *sp << endl;
    weak_ptr<int> wp(sp); //从shared_ptr创建weak_ptr
    assert(wp.use_count() == 1);
    if (!wp.expired())//判断weak_ptr观察的对象是否失效
    {
        shared_ptr<int> sp2 = wp.lock();//获得一个shared_ptr
        cout << *sp2 << endl;
        *sp2 = 100;
        assert(wp.use_count() == 2);
        cout << *sp2 << endl;
    }
    assert(wp.use_count() == 1);
    return 0;
}
