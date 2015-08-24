#include "Proxy.h"
#include <stdlib.h>

int main()
{
    Subject* pProxy = new Proxy();
    pProxy->Request();

    delete pProxy;
    return 0;
}
