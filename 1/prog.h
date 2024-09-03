#ifndef PROG_H
#define PROG_H

#include <iostream>
#include <string>
#include <limits>
#include <cstring>
#include <cmath>

namespace lab1
{
    struct Student
    {
        char *name;
        char group[8];
        double grade;
    };

    std::string struct_to_string(const Student &, const char *);
    std::string struct_to_string(const Student &, const char *, size_t);
    std::string struct_to_string(const Student &, const std::string &);
    Student string_to_struct(const char *);
    Student string_to_struct(const char *, size_t);
    Student string_to_struct(std::string &);
}
#endif