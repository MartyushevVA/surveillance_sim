#ifndef PROG_H
#define PROG_H

#include <iostream>
#include <string>
#include <limits>

namespace lab1
{
    struct Student
    {
        char *name;
        char *group;
        double grade;
    };

    char *struct_to_string(Student, char *);
    Student string_to_struct(char *);
}
#endif