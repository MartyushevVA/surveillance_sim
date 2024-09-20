#include "sclass.h"
#include "hclass.h"

class task
{
};
class stack
{
private:
    size_t size;
    task *space;

public:
    stack()
    {
        this->size = 0;
    }
    stack(size_t size, task space[])
    {
        this->space = new task[size];
        for (size_t i = 0; i < size; i++)
            this->space[i] = space[i];
        this->size = size;
    }

    void operator+=(task t)
    {
        this->size++;
        task *temp = new task[size];
        std::copy(this->space, this->space + this->size, temp);
        delete[] this->space;
        this->space = temp;
        this->space[this->size - 1] = t;
    }

    task pop()
    {
        if (!this->size)
            throw std::runtime_error("Стек пуст");
        this->size--;
        task item = this->space[this->size];
        task *temp = new task[this->size];
        std::copy(this->space, this->space + this->size, temp);
        delete[] this->space;
        this->space = temp;
        return item;
    }

    bool isEmpty() //он как то еще должен быть фуловым
    {
        return !this->size;
    }

    ~stack()
    {
        delete[] space;
    }
};

void stackOut()
{
}

void stackIn()
{
}