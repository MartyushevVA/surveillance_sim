#include "sclass.h"
#include "hclass.h"

class stack
{
private:
    size_t size;
    task vector[ALLCTD]{};
    bool isEmpty() { return size == 0; }

public:
    stack()
    {
        size = 0;
    }
    stack(const size_t size, const task space[])
    {
        std::copy(space, space + size * sizeof(task), this->vector);
        this->size = size;
    }
    void operator+=(const task t)
    {
        if (size == ALLCTD)
            throw std::bad_alloc();
        vector[size++] = t;
    }
    task pop()
    {
        if (isEmpty())
            throw std::runtime_error("Стек пуст");
        task item = vector[--size];
        vector[size] = task();
        return item;
    }
    double fullness()
    {
        return size == 0 ? 0 : (size == ALLCTD ? 2 : 1);
    }
    void unioning()
    {
        size_t pos = size;
        while (pos--)
        {
            task item = pop();
            for (size_t ind = 0; ind < size; ind++) //std::for_each
                if (item.getName() == vector[ind].getName())
                {
                    item = item < vector[ind] ? item + vector[ind] : vector[ind] + item;
                    pos--;
                }
            operator+=(item);
        }
    }
    void fragmentation()
    {
        if (isEmpty())
            throw std::runtime_error("Стек пуст");
        size_t initSize = size;
        size_t pos = 0;
        while (pos++ < initSize) //
        {
            if (vector[pos].getNumOfSheets() == 1)
                continue;
            task *sheets = vector[pos].fragmentation();
            for (size_t ind = 0; ind < vector[pos].getNumOfSheets(); ++ind)
                *this += sheets[ind];
            delete[] sheets;
        }
    }
    task extractNextUngraded()
    {
        if (isEmpty())
            throw std::runtime_error("Стек пуст");
        size_t pos = size - 1;
        while (vector[pos].getGrade()) //std::find_if
            pos--;
        task item = vector[pos];
        std::copy(vector[pos + 1], vector[this->size], vector[pos]);
        vector[--size] = task();
        return item;
    }
};

void stackOut()
{
}

void stackIn()
{
}