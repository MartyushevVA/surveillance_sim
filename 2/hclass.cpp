#include "hclass.h"

void stack::smoothResize(size_t newAllctd = 0)
{
    if (!newAllctd)
    {
        if (size_ * 2 < allctd_)
            newAllctd = allctd_ / 2;
        if (size_ > 4 * allctd_ / 5)
            newAllctd = allctd_ * 2;
        else
            return;
    }
    task *newArray = new task[newAllctd]{};
    if (newArray == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    std::copy(vector_, vector_ + size_, newArray);
    delete[] vector_;
    vector_ = newArray;
    allctd_ = newAllctd;
}

stack::stack()
{
    vector_ = new task[10]{};
    size_ = 0;
    allctd_ = 0;
}

stack::stack(size_t size, const task (&space)[])
{
    smoothResize(2 * size);
    std::copy(space, space + size, vector_);
    size_ = size;
}

stack::stack(const stack &other)
{
    size_ = other.size_;
    allctd_ = other.allctd_;
    vector_ = new task[allctd_];
    std::copy(other.vector_, other.vector_ + other.size_, vector_);
}

stack::stack(stack &&other)
{
    size_ = other.size_;
    allctd_ = other.allctd_;
    vector_ = other.vector_;
    other.size_ = 0;
    other.allctd_ = 0;
    other.vector_ = nullptr;
}

stack::~stack()
{
    delete[] vector_;
}

stack &stack::operator=(const stack &other)
{
    if (this != &other)
    {
        size_ = other.size_;
        allctd_ = other.allctd_;
        vector_ = new task[allctd_];
        std::copy(other.vector_, other.vector_ + other.size_, vector_);
    }
    return *this;
}

stack &stack::operator+=(const task &t)
{
    smoothResize();
    vector_[size_++] = t;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const stack &st)
{
    std::string output;
    stack copied = stack(st);
    task item;
    for (size_t num = 0; num < st.size_; num++)
    {
        item = copied.pop();
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
    if (size_ == 0)
        throw ofuncs::EmptyStackException("Current stack is empty.");
    task item = vector_[--size_];
    vector_[size_] = task();
    return item;
}

int stack::fullness() const { return size_ == 0 ? 0 : (size_ == allctd_ ? 2 : 1); }

void stack::unioning()
{
    task *buf = new task[allctd_]{};
    if (buf == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    task item;
    size_t edge = 0;
    while (size_)
    {
        item = this->pop();
        size_t samepos = ofuncs::find(buf, edge, item);
        if (samepos != edge)
            buf[samepos] = item < buf[samepos] ? item + buf[samepos] : buf[samepos] + item;
        else
            buf[edge++] = item;
    }
    std::copy(buf, buf + edge, vector_);
    delete[] buf;
    size_ = edge;
}

void stack::fragmentation()
{
    size_t bufallctd = allctd_;
    task *buf = new task[bufallctd]{};
    if (buf == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    task item;
    size_t edge = 0;
    while (size_)
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
    std::copy(buf, buf + edge, vector_);
    delete[] buf;
    size_ = edge;
}

task stack::extractNextUngraded()
{
    task *buf = new task[allctd_]{};
    if (buf == nullptr)
        throw std::runtime_error("Memory allocation failed.");
    task item;
    size_t edge = 0;
    if (size_ == 0)
        throw ofuncs::EmptyStackException("Current stack is empty.");
    while (size_)
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