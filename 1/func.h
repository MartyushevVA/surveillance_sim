#ifndef FUNC_H
#define FUNC_H

#include <string>
#include <cmath>

namespace lab1
{
    struct Student
    {
        std::string name;
        std::string group;
        double grade;
    };

    std::string struct_to_string(const Student &, const char *);
    std::string struct_to_string(const Student &, const char *, size_t);
    std::string struct_to_string(const Student &, const std::string &);
    Student string_to_struct(const char *);
    Student string_to_struct(const char *, size_t);
    Student string_to_struct(const std::string);
}
#endif