#ifndef SCLASS_H
#define SCLASS_H

#include "alllibs.h"

class task
{
private:
    std::string name;
    int grade;
    size_t first;
    size_t last;

public:
    task();
    task(std::string name, int grade, size_t first, size_t last);
    task(std::string name);
    std::string getName() const;
    int getGrade() const;
    size_t getFirst() const;
    size_t getLast() const;
    void setName(std::string name);
    void setGrade(int grade);
    void setFirst(size_t first);
    void setLast(size_t last);
    task operator+(const task &t) const;
    bool operator==(const task &t) const;
    bool operator<(const task &t) const;
    void evaluate(int grade);
    task *fragmentation() const;
    int getNumOfSheets() const;
};

namespace scofuncs
{
    size_t find(const task space[], size_t size, task findable);
    void copy(task *destination, task source[], size_t sourceSize);
};

#endif