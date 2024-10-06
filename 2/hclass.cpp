#include "hclass.h"

void stack::smoothResize(size_t newAllctd = 0){
    if (newAllctd == 0){
        if (size_ * 2 < allctd_)
            newAllctd = allctd_ / 2;
        else if (size_ > 4 * allctd_ / 5)
            newAllctd = allctd_ * 2;
        else
            return;
    }
    if (newAllctd < 0)
        throw std::range_error("Can't be less than zero.");
    task *newArray = new task[newAllctd]{};
    std::copy(vector_, vector_ + std::min(size_, newAllctd), newArray);
    delete[] vector_;
    vector_ = newArray;
    allctd_ = newAllctd;
}

stack::stack() {}

stack::stack(size_t size, const task (&space)[]){
    size_ = size;
    smoothResize(size);
    std::copy(space, space + size, vector_);
}

stack::stack(const stack &other){
    size_ = other.size_;
    smoothResize(other.allctd_);
    std::copy(other.vector_, other.vector_ + other.size_, vector_);
}

stack::stack(stack &&other) noexcept{
    delete[] vector_;
    size_ = other.size_;
    allctd_ = other.allctd_;
    vector_ = other.vector_;
    other.size_ = 0;
    other.allctd_ = 0;
    other.vector_ = nullptr;
}

stack::~stack(){
    delete[] vector_;
}

size_t stack::getSize() const { return size_; }

size_t stack::getAllctd() const { return allctd_; }

task* stack::getVector() const { return vector_; }

stack &stack::operator=(const stack &other){
    if (this != &other){
        delete[] vector_;
        size_ = other.size_;
        allctd_ = other.allctd_;
        vector_ = new task[allctd_];
        std::copy(other.vector_, other.vector_ + other.size_, vector_);
    }
    return *this;
}

stack &stack::operator=(stack &&other) noexcept{
    if (this != &other){
        delete[] vector_;
        size_ = other.size_;
        allctd_ = other.allctd_;
        vector_ = other.vector_;
        other.size_ = 0;
        other.allctd_ = 0;
        other.vector_ = nullptr;
    }
    return *this;
}

stack &stack::operator++(){
    smoothResize(allctd_+1);
    return *this;
}

stack stack::operator++(int){
    stack temp = *this;
    smoothResize(allctd_+1);
    return temp;
}

stack &stack::operator--(){
    smoothResize(allctd_-1);
    return *this;
}

stack stack::operator--(int){
    stack temp = *this;
    smoothResize(allctd_-1);
    return temp;
}

stack &stack::operator+=(const task &t){
    smoothResize();
    vector_[size_++] = t;
    return *this;
}

const task& stack::operator[](size_t position) const{
    return vector_[position];
}

task& stack::operator[](size_t position){
    return vector_[position];
}

std::ostream &operator<<(std::ostream &os, const stack &st){
    std::string output;
    stack copied = stack(st);
    task item;
    for (size_t num = 0; num < st.size_; num++)
        os << st.getVector()[num] << std::endl;
    return os;
}

std::istream &operator>>(std::istream &in, stack &stack){
    size_t size;
    task item;
    in >> size;
    for (size_t ind = 0; ind < size; ind++){
        in >> item;
        stack += item;
    }
    return in;
}

task stack::pop(){
    if (size_ == 0)
        throw ofuncs::EmptyStackException("Current stack is empty.");
    task item = vector_[--size_];
    smoothResize();
    return item;
}

int stack::fullness() const { return size_ == 0 ? 0 : (size_ == allctd_ ? 2 : 1); }

void stack::unioning(){
    task *buf = new task[allctd_]{};
    if (buf == nullptr)
        throw std::bad_alloc();
    task item;
    size_t edge = 0;
    while (size_){
        item = this->pop();
        size_t samepos = ofuncs::find(buf, edge, item);
        if (samepos != edge)
            buf[samepos] = item < buf[samepos] ? item + buf[samepos] : buf[samepos] + item;
        else
            buf[edge++] = item;
    }
    smoothResize(edge);
    std::copy(buf, buf + edge, vector_);
    delete[] buf;
    size_ = edge;
}

void stack::fragmentation(){
    size_t bufallctd = allctd_;
    task *buf = new task[bufallctd];
    if (!buf)
        throw std::bad_alloc();
    task item;
    size_t edge = 0;
    while (size_){
        item = this->pop();
        task *sheets = item.fragmentation();
        size_t numOfSheets = item.getNumOfSheets();
        for (size_t i = 0; i < numOfSheets; ++i){
            if (edge == bufallctd){
                bufallctd *= 2;
                task* newBuf = new task[bufallctd];
                if (!newBuf){
                    delete[] buf;
                    delete[] sheets;
                    throw std::bad_alloc();
                }
                std::copy(buf, buf + edge, newBuf);
                delete[] buf;
                buf = newBuf;
            }
            buf[edge++] = sheets[i];
        }
        delete[] sheets;
    }
    smoothResize(2 * edge);
    std::copy(buf, buf + edge, vector_);
    delete[] buf;
    size_ = edge;
}

task stack::extractNextUngraded(){
    if (size_ == 0)
        throw ofuncs::EmptyStackException("Current stack is empty.");
    task *buf = new task[allctd_]{};
    if (!buf)
        throw std::bad_alloc();
    task item;
    size_t edge = 0;
    while (size_){
        item = this->pop();
        if (item.getGrade() == 0) {
            std::for_each(buf, buf + edge, [this](task t)
            { *this += t; });
            delete[] buf;
            return item;
        }
        buf[edge++] = item;
    }
    delete[] buf;
    throw ofuncs::TaskNotFoundException("There's no ungraded works.");
}