#include "sclass.h"

size_t ofuncs::find(const task space[], size_t size, const task &findable){
    if (space == nullptr)
        throw std::runtime_error("The task space pointer is null.");
    for (size_t ind = 0; ind < size; ++ind)
        if (space[ind].getName() == findable.getName())
            return ind;
    return size;
}

task::task() {}

task::task(std::string name, int grade, size_t first, size_t last){
    setName(name);
    setGrade(grade);
    setFirst(first);
    setLast(last);
}

task::task(std::string name) { setName(name); }

std::string task::getName() const { return name_; }

int task::getGrade() const { return grade_; }

size_t task::getFirst() const { return first_; }

size_t task::getLast() const { return last_; }

void task::setName(std::string name) { name_ = name; }

void task::setGrade(int grade){
    if ((grade < 2 || grade > 5) && grade != 0)
        throw std::range_error("Out of range");
    grade_ = grade;
}

void task::setFirst(size_t first){
    if (last_ != 0 && last_ < first)
        throw std::range_error("Out of range");
    first_ = first;
}

void task::setLast(size_t last){
    if (first_ > last)
        throw std::range_error("Out of range");
    last_ = last;
}

task task::operator+(const task &t) const {
    bool conditions[2]{name_ != t.getName(), t.first_ != last_ + 1};
    if (std::any_of(conditions, conditions + 2, [](bool cond) { return cond; }))
        throw ofuncs::WrongPositioningException("Unable to combine these works.");
    return task(name_, grade_, first_, t.getLast());
}

bool task::operator==(const task &t) const {
    return name_ == t.getName() && first_ == t.getFirst();
}

bool task::operator<(const task &t) const {
    if (name_ < t.getName())
        return true;
    if (name_ == t.getName() && first_ < t.getFirst())
        return true;
    return false;
}

std::strong_ordering task::operator<=>(const task &t) const {
    if (auto cmp = name_ <=> t.getName(); cmp != 0)
        return cmp;
    return first_ <=> t.getFirst();
}

task task::operator!() const {
    return task(name_, 5 - grade_, first_, last_);
}

void task::evaluate(int grade) {
    if (grade < 2 || grade > 5)
        throw ofuncs::UnexpectedGradeException("Unable to mark work like that.");
    grade_ = grade;
}

task *task::fragmentation() const {
    task *sheets = new task[last_ - first_ + 1];
    if (sheets == nullptr)
        throw std::bad_alloc();
    size_t pointer = 0;
    for (size_t i = first_; i <= last_; i++)
        sheets[pointer++] = task(name_, 0, i, i);
    sheets[0].grade_ = grade_;
    return sheets;
}

int task::getNumOfSheets() const { return last_ - first_ + 1; }

std::ostream &operator<<(std::ostream &os, const task &task) {
    return os << task.getName() << ": " << task.getGrade() << " " << task.getFirst() << "<->" << task.getLast();
}

std::istream &operator>>(std::istream &in, task &task) {
    std::string name;
    int grade;
    size_t first, last;
    in >> name >> grade >> first >> last;
    if (in){
        task.setName(name);
        task.setGrade(grade);
        task.setFirst(first);
        task.setLast(last);
    }
    return in;
}