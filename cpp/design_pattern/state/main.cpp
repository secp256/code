#include "state.h"

int main()
{
    State *pState = new ConcreateStateA();
    Context *pContext = new Context(pState);
    pContext->Request();
    pContext->Request();
    pContext->Request();

    delete pContext;

    return 0;
}
