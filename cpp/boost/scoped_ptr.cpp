#include <iostream>
#include <boost/scoped_ptr.hpp>

using namespace std;

class impl
{
public:
    ~impl() { cout << "destroying impl" << endl; }
    void do_something() { cout << "do something" << endl; }
};

int main()
{
    boost::scoped_ptr<impl> imp(new impl());
    imp->do_something();

    return 0;
}
