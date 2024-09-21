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
    if (newArray == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    std::copy(vector, vector + size, newArray);
    delete[] vector;
    vector = newArray;
    allctd = newAllctd;
}

stack::stack()
{
    vector = new task[10]{};
    size = 0;
    allctd = 0;
}

stack::stack(size_t size, const task (&space)[])
{
    smoothResize(2 * size);
    std::copy(space, space + size, vector);
    this->size = size;
}

stack::stack(const stack &other)
{
    size = other.size;
    allctd = other.allctd;
    vector = new task[allctd];
    std::copy(other.vector, other.vector + other.size, vector);
}

stack::stack(stack &&other)
{
    size = other.size;
    allctd = other.allctd;
    vector = other.vector;
    other.size = 0;
    other.allctd = 0;
    other.vector = nullptr;
}

stack::~stack()
{
    delete[] vector;
}

stack &stack::operator=(const stack &other)
{
    if (this != &other)
    {
        size = other.size;
        allctd = other.allctd;
        vector = new task[allctd];
        std::copy(other.vector, other.vector + other.size, vector);
    }
    return *this;
}

void stack::operator+=(const task &t)
{
    smoothResize();
    vector[size++] = t;
}

std::ostream &operator<<(std::ostream &os, const stack &stack)
{
    std::string output;
    task item;
    for (size_t num = 0; num < stack.size; num++)
    {
        item = stack.vector[num];
        output += item.getName() += std::string(": ") += std::to_string(item.getGrade()) += std::string(" ") += std::to_string(item.getFirst()) += std::string("<->") += std::to_string(item.getLast()) += "\n";
    }
    return os << output;
}

std::istream &operator>>(std::istream &in, stack &stack)
{
    size_t size;
    task item;
    in >> size;
    for (size_t ind = 0; ind < size; ind++)
    {
        in >> item;
        stack.smoothResize();
        stack += item;
    }
    return in;
}

task stack::pop()
{
    if (size == 0)
        throw ofuncs::EmptyStackException("Current stack is empty.");
    task item = vector[--size];
    vector[size] = task();
    return item;
}

int stack::fullness() const { return size == 0 ? 0 : (size == allctd ? 2 : 1); }

void stack::unioning()
{
    task *buf = new task[allctd]{};
    if (buf == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    task item;
    size_t edge = 0;
    while (size)
    {
        item = this->pop();
        size_t samepos = ofuncs::find(buf, edge, item);
        if (samepos != edge)
            buf[samepos] = item < buf[samepos] ? item + buf[samepos] : buf[samepos] + item;
        else
            buf[edge++] = item;
    }
    ofuncs::copy(vector, buf, edge);
    size = edge;
}

void stack::fragmentation()
{
    size_t bufallctd = allctd;
    task *buf = new task[bufallctd]{};
    if (buf == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    task item;
    size_t edge = 0;
    while (size)
    {
        item = this->pop();
        task *sheets = item.fragmentation();
        std::for_each(sheets, sheets + item.getNumOfSheets() * sizeof(task), [&buf, &edge, &bufallctd, &sheets](task sheet)
                      {
            if (edge == bufallctd)
            {
                bufallctd*=2;
                task* newBuf = new task[bufallctd]{};
                if (newBuf == nullptr)
                {
                    delete[] buf;
                    delete[] sheets;
                    throw std::runtime_error("Memory allocation failed.");
                }
                std::copy(buf, buf+edge, newBuf);
                delete[] buf;
                buf = newBuf;
            }
            buf[edge++] = sheet; });
        delete[] sheets;
    }
    smoothResize(2 * edge);
    ofuncs::copy(vector, buf, edge);
    size = edge;
}

task stack::extractNextUngraded()
{
    task *buf = new task[allctd]{};
    if (buf == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    task item;
    size_t edge = 0;
    if (size == 0)
        throw ofuncs::EmptyStackException("Current stack is empty.");
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
    throw ofuncs::TaskNotFoundException("There's no ungraded works.");
}

void stackOut()
{
}

void stackIn()
{
}