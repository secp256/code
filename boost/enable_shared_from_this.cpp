#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost;
using namespace std;

// g++ enable_shared_from_this.cpp -lboost_system

class self_shared: public enable_shared_from_this<self_shared>
{
public:
    self_shared(int n):x(n){}
    int x;
    void print()
    {cout<<" self_shared:"<<x<<endl;}
};

int main()
{
    shared_ptr<self_shared> sp = make_shared<self_shared>(314);
    sp->print();

    // shared_from_this()会返回this的shared_ptr.
    shared_ptr<self_shared> p = sp->shared_from_this();
    p->x = 1000;
    p->print();
    return 0;
}
