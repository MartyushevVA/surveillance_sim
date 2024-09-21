#ifndef SCLASS_H
#define SCLASS_H

#include <algorithm>
#include <stdexcept>
#include <string>

class task
{
public:
    task();
    task(std::string, int, size_t, size_t);
    task(std::string);
    std::string getName();
    int getGrade();
    size_t getFirst();
    size_t getLast();
    void setName(std::string);
    void setGrade(int);
    void setFirst(size_t);
    void setLast(size_t);
    task operator+(task);
    bool operator==(task);
    bool operator<(task);
    void evaluate(int);
    task *fragmentation();
    int getNumOfSheets();
};

#endif