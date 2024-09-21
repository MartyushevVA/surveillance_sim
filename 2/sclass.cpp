#include "sclass.h"

class task
{
private:
    std::string name;
    int grade;
    size_t first;
    size_t last;

public:
    task()
    {
        name = "none";
        grade = 0;
        first = 0;
        last = 0;
    }
    task(std::string name, int grade, size_t first, size_t last)
    {
        this->name = name;
        this->grade = grade;
        this->first = first;
        this->last = last;
    }
    task(std::string name)
    {
        this->name = name;
        this->grade = 0;
        this->first = 0;
        this->last = 0;
    }
    std::string getName() const { return name; }
    int getGrade() const { return grade; }
    size_t getFirst() const { return first; }
    size_t getLast() const { return last; }
    void setName(const std::string name) { this->name = name; }
    void setGrade(const int grade) { this->grade = grade; }
    void setFirst(const size_t first) { this->first = first; }
    void setLast(const size_t last) { this->last = last; }
    task operator+(const task t)
    {
        bool conditions[2]{this->name != t.getName(), t.first != this->last + 1};
        if (std::any_of(std::begin(conditions), std::end(conditions), [](bool cond)
                        { return cond; }))
            throw std::runtime_error("Невозможно объединить такие работы");
        return task(this->name, this->grade, this->first, t.getLast());
    }
    bool operator==(const task t)
    {
        return this->name == t.getName() && this->first == t.getFirst();
    }
    bool operator<(const task t)
    {
        if (this->name >= t.getName())
            return false;
        if (this->first >= t.getFirst())
            return false;
        return true;
    }
    void evaluate(const int grade)
    {
        if (grade < 2 && grade > 5)
            throw std::runtime_error("Невозможно поставить такую оценку");
        this->grade = grade;
    }
    task *fragmentation()
    {
        task *sheets = new task[last - first];
        size_t pointer = 0;
        for (size_t i = first; i < last; i++)
            sheets[pointer++] = task(name, 0, i, i);
        sheets[0].grade = grade;
        return sheets;
    }
    int getNumOfSheets()
    {
        return this->last - this->first + 1;
    }
};

void taskOut()
{
}

void taskIn()
{
}