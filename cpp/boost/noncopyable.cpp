#include <iostream>
#include <boost/utility.hpp>

using namespace std;

class DoNotCopy : boost::noncopyable {
public:
    DoNotCopy() {};
    DoNotCopy(int i) {};

};

int main()
{
    DoNotCopy d1;
    DoNotCopy d2(d1);
    // DoNotCopy d3 = d1;
    return 0;
}
