#ifndef HCLASS_H
#define HCLASS_H

#include <string>

class stack
{
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