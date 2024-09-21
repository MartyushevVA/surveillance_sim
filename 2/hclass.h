#ifndef HCLASS_H
#define HCLASS_H
#define ALLCTD 300

#include "alllibs.h"
#include "sclass.h"

class stack
{
private:
    size_t size;
    task vector[];

public:
    stack();
    stack(size_t size, const task (&space)[]);
    void operator+=(const task &t);
    task pop();
    int fullness() const;
    void unioning();
    void fragmentation();
    task extractNextUngraded();
};

#endif