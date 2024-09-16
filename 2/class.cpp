#include <string>

class task
{
private:
    std::string name;
    int grade;
    size_t begin;
    size_t end;

public:
    std::string getName() { return name; }
    int getGrade() { return grade; }
    size_t begin() { return begin; }
    size_t end() { return end; }
    void setName(std::string name) { this->name = name; }
    void setGrade(int grade) { this->grade = grade; }
    void setBegin(size_t begin) { this->begin = begin; }
    void setEnd(size_t end) { this->end = end; }
}