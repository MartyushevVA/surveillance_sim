#ifndef HCLASS_H
#define HCLASS_H

#include "alllibs.h"
#include "sclass.h"

class stack
{
private:
    size_t size;
    size_t allctd;
    task* vector;

public:
    stack();
    stack(size_t size, const task (&space)[]);
    ~stack();
    void operator+=(const task &t);
    task pop();
    int fullness() const;
    void unioning();
    void fragmentation();
    task extractNextUngraded();
    void smoothResize(size_t hardSize);
};

#endif