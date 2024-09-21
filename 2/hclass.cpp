#include "hclass.h"

void stack::smoothResize(size_t newAllctd = 0)
{
    if (!newAllctd)
    {
        if (size * 2 < allctd)
            newAllctd = allctd / 2;
        if (size > 4 * allctd / 5)
            newAllctd = allctd * 2;
        else
            return;
    }
    task *newArray = new task[newAllctd]{};
    std::copy(vector, vector + size, newArray);
    delete[] vector;
    vector = newArray;
    allctd = newAllctd;
}

stack::stack()
{
    task *vector = new task[10]{};
    size = 0;
    allctd = 0;
}

stack::stack(size_t size, const task (&space)[])
{
    smoothResize(2 * size);
    std::copy(space, space + size, vector);
    this->size = size;
}

stack::~stack()
{
    delete[] vector;
}

void stack::operator+=(const task &t)
{
    smoothResize();
    vector[size++] = t;
}

task stack::pop()
{
    if (size == 0)
        throw std::runtime_error("Стек пуст");
    task item = vector[--size];
    vector[size] = task();
    return item;
}

int stack::fullness() const { return size == 0 ? 0 : (size == allctd ? 2 : 1); }

void stack::unioning()
{
    task *buf = new task[allctd]{};
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
    scofuncs::copy(vector, buf, edge);
    size = edge;
}

void stack::fragmentation()
{
    size_t bufallctd = allctd;
    task *buf = new task[bufallctd]{};
    task item;
    size_t edge = 0;
    while (size)
    {
        item = this->pop();
        task *sheets = item.fragmentation();
        std::for_each(sheets, sheets + item.getNumOfSheets() * sizeof(task), [&buf, &edge, &bufallctd](task sheet)
                      {
            if (edge == bufallctd)
            {
                bufallctd*=2;
                task* newBuf = new task[bufallctd]{};
                std::copy(buf, buf+edge, newBuf);
                delete[] buf;
                buf = newBuf;
            }
            buf[edge++] = sheet; });
        delete[] sheets;
    }
    smoothResize(2 * edge);
    scofuncs::copy(vector, buf, edge);
    size = edge;
}

task stack::extractNextUngraded()
{
    task *buf = new task[allctd]{};
    task item;
    size_t edge = 0;
    if (size == 0)
        throw std::runtime_error("Стек пуст");
    while (size)
    {
        item = this->pop();
        if (item.getGrade())
            buf[edge++] = item;
        else
        {
            std::for_each(buf, buf + edge, [this](task t)
                          { *this += (t); });
            return item;
        }
    }
    throw std::runtime_error("Неоцененных работ нет");
}

void stackOut()
{
}

void stackIn()
{
}