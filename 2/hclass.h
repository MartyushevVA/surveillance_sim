#ifndef HCLASS_H
#define HCLASS_H
#define ALLCTD 100
#include <string>

class stack
{
public:
    stack();
    stack(const size_t, const task[]);
    void operator+=(const task &);
    task pop();
    double fullness();
    void unioning();
    task *fragmentation();
    task extractNextUngraded();
};

#endif