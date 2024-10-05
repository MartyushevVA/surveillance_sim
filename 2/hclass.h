#ifndef HCLASS_H
#define HCLASS_H

#include "alllibs.h"
#include "sclass.h"

class stack
{
    friend std::ostream &operator<<(std::ostream &os, const stack &stack);
    friend std::istream &operator>>(std::istream &in, stack &stack);

private:
    size_t size_ = 0;
    size_t allctd_ = 10;
    task *vector_ = new task[allctd_];

public:
    stack();
    stack(size_t size, const task (&space)[]);
    stack(const stack &other);
    stack(stack &&other) noexcept;
    ~stack();
    size_t getSize() const;
    size_t getAllctd() const;
    task* getVector() const;
    stack &operator=(const stack &other);
    stack &operator=(stack &&other) noexcept;
    stack &operator+=(const task &t);
    stack &operator++();
    stack operator++(int);
    stack &operator--();
    stack operator--(int);
    const task &operator[](size_t position) const;
    task &operator[](size_t position);
    task pop();
    int fullness() const;
    void unioning();
    void fragmentation();
    task extractNextUngraded();
    void smoothResize(size_t hardSize);
};

std::ostream &operator<<(std::ostream &os, const stack &stack);
std::istream &operator>>(std::istream &in, stack &stack);

#endif