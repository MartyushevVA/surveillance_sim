#include "hclass.h"
#include "sclass.h"

class stack
{
private:
    size_t size;
    task *space;

public:
    stack()
    {
        size = 0;
    }
    stack(size_t size, task space[])
    {
        
    }

    stack operator+(stack t)
    {
        bool conditions[2]{this->name != t.getName(), t.first != this->last + 1};
        if (std::any_of(std::begin(conditions), std::end(conditions), [](bool cond)
                        { return cond; }))
            throw std::runtime_error("Невозможно объединить такие работы");
        return stack(this->name, this->grade, this->first, t.getLast());
    }

    bool operator==(stack t)
    {
        return this->name == t.getName() && this->first == t.getFirst();
    }

    bool operator<(stack t)
    {
        if (this->name >= t.getName())
            return false;
        if (this->first >= t.getFirst())
            return false;
        return true;
    }

    void evaluate(int grade)
    {
        if (grade < 2 && grade > 5)
            throw std::runtime_error("Невозможно поставить такую оценку");
        this->grade = grade;
    }

    stack *fragmentation()
    {
        stack *sheets = new stack[last - first];
        size_t pointer = 0;
        for (size_t i = first; i < last; i++)
            sheets[pointer++] = stack(name, 0, i, i);
        return sheets;
    }
};

void stackOut()
{
}

void stackIn()
{
}