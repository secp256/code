#include "Decorator.h"
#include <stdlib.h>

int main()
{
    // 初始化一个Component对象
    Component* pComponent = new ConcreateComponent();
    // 采用这个Component对象去初始化一个Decorator对象,
    // 这样就可以为这个Component对象动态添加职责
    Decorator* pDecorator = new ConcreateDecorator(pComponent);
    pDecorator->Operation();

    delete pDecorator;
    return 0;
}
