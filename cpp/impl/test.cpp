#include <iostream>
#include "parent.h"

using std::cout;
using std::endl;


int main()
{
    int i;
    short j;
    parent* p = get_child();

    p->set(2,3);
    p->get(&i, &j);

    cout << i << endl;
    cout << j << endl;
    return 0;
}
