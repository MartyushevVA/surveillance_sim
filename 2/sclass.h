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
    std::string getName() const;
    int getGrade() const;
    size_t getFirst() const;
    size_t getLast() const;
    void setName(const std::string);
    void setGrade(const int);
    void setFirst(const size_t);
    void setLast(const size_t);
    task operator+(const task);
    bool operator==(const task);
    bool operator<(const task);
    void evaluate(const int);
    task *fragmentation();
    int getNumOfSheets();
};

#endif