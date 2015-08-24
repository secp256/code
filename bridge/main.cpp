#include "Bridge.h"
#include <stdlib.h>
#include <stdio.h>
int main()
{
    Implementor *pImplA = new ConcreateImplementorA();
    Abstraction *pAbstraction1 = new Abstraction(pImplA);
    pAbstraction1->Operation();

    Implementor *pImplB = new ConcreateImplementorB();
    Abstraction *pAbstraction2 = new Abstraction(pImplB);
    pAbstraction2->Operation();

    delete pAbstraction1;
    delete pAbstraction2;

    return 0;
}
