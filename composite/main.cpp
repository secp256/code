#include "Composite.h"
#include <stdlib.h> 
int main()
{ 
    Composite* pComposite = new Composite("Compostie");
    pComposite->Add(new Leaf("leaf1"));
    pComposite->Add(new Leaf("leaf2"));

    pComposite->Operation();
    pComposite->GetChild(1)->Operation();
    delete pComposite;

    Composite* myComposite = new Composite("myCompostie");
    myComposite->Add(new Leaf("leaf3"));
    myComposite->Operation();
    delete myComposite;

    return 0;
}
