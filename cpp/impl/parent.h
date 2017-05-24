#ifndef PARENT_H
#define PARENT_H
class parent {
public:
    virtual void set(int i, short j) = 0;
    virtual void get(int *i, short *j) = 0;
};

parent* get_child();
#endif
