#include "sclass.h"
#include "hclass.h"
#define ALLCTD 300

namespace hcofuncs
{
}

class stack
{
private:
    size_t size;
    task vector[];

public:
    stack()
    {
        vector[ALLCTD] = {};
        size = 0;
    }
    stack(size_t size, const task (&space)[])
    {
        std::copy(space, space + size * sizeof(task), vector);
        this->size = size;
    }
    void operator+=(const task &t)
    {
        if (size == ALLCTD)
            throw std::bad_alloc();
        vector[size++] = t;
    }
    task pop()
    {
        if (size == 0)
            throw std::runtime_error("Стек пуст");
        task item = vector[--size];
        vector[size] = task();
        return item;
    }
    int fullness() const
    {
        return size == 0 ? 0 : (size == ALLCTD ? 2 : 1);
    }
    void unioning()
    {
        task buf[ALLCTD] = {};
        task item;
        size_t edge = 0;
        while (size)
        {
            item = this->pop();
            size_t samepos = scofuncs::find(buf, edge, item);
            if (samepos != edge)
                buf[samepos] = item < buf[samepos] ? item + buf[samepos] : buf[samepos] + item;
            else
                buf[edge++] = item;
        }
        delete[] vector;
        scofuncs::copy(vector, buf, edge);
        size = edge;
    }
    void fragmentation()
    {
        task buf[ALLCTD] = {};
        task item;
        size_t edge = 0;
        while (size)
        {
            item = this->pop();
            task *sheets = item.fragmentation();
            std::for_each(sheets, sheets + item.getNumOfSheets() * sizeof(task), [&buf, &edge](task sheet)
                          { buf[edge++] = sheet; });
            delete[] sheets;
        }
        delete[] vector;
        scofuncs::copy(vector, buf, edge);
        size = edge;
    }
    task extractNextUngraded()
    {
        task buf[ALLCTD] = {};
        task item;
        size_t edge = 0;
        while (size)
        {
            item = this->pop();
            if (item.getGrade())
                buf[edge++] = item;
            else
            {
                std::for_each(buf, buf + edge * sizeof(task), [this](task t)
                              { *this += (t); });
                delete[] buf;
                return item;
            }
        }
    }
};

void stackOut()
{
}

void stackIn()
{
}