#include "sclass.h"
#include "hclass.h"

class stack
{
private:
    size_t size;
    task vector[ALLCTD]{};
    bool isEmpty() { return !size; }

public:
    stack()
    {
        this->size = 0;
    }
    stack(const size_t size, const task space[])
    {
        std::copy(space, space + size * sizeof(task), this->vector);
        this->size = size;
    }
    void operator+=(const task t)
    {
        if (this->size == ALLCTD)
            throw std::bad_alloc();
        this->vector[this->size++] = t;
    }
    task pop()
    {
        if (isEmpty())
            throw std::runtime_error("Стек пуст");
        task item = this->vector[--this->size];
        this->vector[this->size] = task();
        return item;
    }
    double fullness()
    {
        return !this->size ? 0 : (this->size == ALLCTD ? 2 : 1);
    }
    void unioning()
    {
        size_t pos = this->size;
        while (pos--)
        {
            task item = this->pop();
            for (size_t ind = 0; ind < this->size; ind++)
                if (item.getName() == this->vector[ind].getName())
                {
                    item = item < this->vector[ind] ? item + this->vector[ind] : this->vector[ind] + item;
                    pos--;
                }
            this->operator+=(item);
        }
    }
    void fragmentation()
    {
        if (isEmpty())
            throw std::runtime_error("Стек пуст");
        size_t initSize = this->size;
        size_t pos = 0;
        while (pos++ < initSize)
        {
            if (this->vector[pos].getNumOfSheets() == 1)
                continue;
            task *sheets = this->vector[pos].fragmentation();
            for (size_t ind = 0; ind < this->vector[pos].getNumOfSheets(); ++ind)
                *this += sheets[ind];
            delete[] sheets;
        }
    }
    task extractNextUngraded()
    {
        if (isEmpty())
            throw std::runtime_error("Стек пуст");
        size_t pos = this->size - 1;
        while (this->vector[pos].getGrade())
            pos--;
        task item = this->vector[pos];
        std::copy(this->vector[pos + 1], this->vector[this->size], this->vector[pos]);
        this->vector[--this->size] = task();
        return item;
    }
};

void stackOut()
{
}

void stackIn()
{
}