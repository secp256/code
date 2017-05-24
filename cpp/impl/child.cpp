#include "child.h"

child::child()
{
    _i = 0;
    _j = 0;
}

child::~child()
{
    _i = 0;
    _j = 0;
}

void child::set(int i, short j)
{
    _i = i;
    _j = j;
}

void child::get(int *i, short *j)
{
    *i = _i;
    *j = _j;
}
