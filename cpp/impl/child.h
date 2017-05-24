#ifndef CHILD_H
#define CHILD_H
#include "parent.h"

class child : public parent {
private:
    int _i;
    short _j;
public:
    child();
    ~child();

    virtual void set(int i, short j);
    virtual void get(int *i, short *j);
};
#endif
