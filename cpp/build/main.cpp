#include "Builder.h"
#include <stdlib.h>

int main()
{
    Builder* pBuilder1 = new ConcreateBuilder1;
    Director *pDirector1 = new Director(pBuilder1);
    pDirector1->Construct();

    Builder* pBuilder2 = new ConcreateBuilder2;
    Director *pDirector2 = new Director(pBuilder2);
    pDirector2->Construct();

    delete pDirector1;
    delete pDirector2;
    return 0;
}
