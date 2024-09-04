#include "prog.h"
using namespace lab1;

char *my_realloc(char *src, size_t oldSize, size_t newSize)
{
    char *newArr = new char[newSize]{};
    for (int i = 0; i < (newSize > oldSize ? oldSize : newSize); i++)
        newArr[i] = src[i];
    delete[] src;
    return newArr;
}

void my_concat(char *dest, const char *src)
{
    int pos = 0, i = 0;
    while (dest[pos])
        pos++;
    while (src[i])
        dest[pos + i] = src[i++];
}

void my_concat(char *dest, const char *src, size_t size)
{
    int pos = 0;
    while (dest[pos])
        pos++;
    for (int i = 0; i < size; i++)
        dest[pos + i] = src[i];
}

size_t my_strlen(const char *string)
{
    size_t size = 0;
    while (string[size])
        size++;
    return size;
}

char *double_to_string(double value)
{
    char *str = new char[10]{};
    int intpart = (int)value;
    double doubpart = value - intpart;
    str[0] = intpart + '0';
    str[1] = '.';
    for (int i = 2; i < 6; i++)
    {
        doubpart *= 10;
        int doubt = (int)doubpart;
        str[i] = doubt + '0';
        doubpart -= doubt;
    }
    return my_realloc(str, 10, strlen(str) + 1);
}

std::string lab1::struct_to_string(const Student &student, const char *title)
{
    std::string ttl = std::string(title);
    return struct_to_string(student, ttl);
}

std::string lab1::struct_to_string(const Student &student, const char *title, size_t length)
{
    std::string ttl = std::string(title, length);
    return struct_to_string(student, ttl);
}

std::string lab1::struct_to_string(const Student &student, const std::string &title)
{
    std::string java_prop;
    java_prop += title;
    java_prop += ".name=";
    java_prop += student.name;
    java_prop += "\n";
    java_prop += title;
    java_prop += ".group=";
    java_prop += student.group;
    java_prop += "\n";
    java_prop += title;
    java_prop += ".grade=";
    java_prop += std::to_string(student.grade);
    java_prop += "\n";
    return java_prop;
}

char *get_info_after(std::string src, const char *infotype)
{
    static int pos = 0;
    int length = 0;
    while (src[pos] != '=')
        pos++;
    pos++;
    while (src[pos + length] != '\n')
        length++;
    char *data = new char[length + 1]{};
    for (int i = 0; i < length; i++)
        data[i] = src[pos + i];
    return data;
}

double string_to_double(char *value)
{
    int multiplier = 0;
    double res = value[0] - '0';
    for (int i = 0; i < strlen(value); i++)
        res = (value[i] - '0') / (std::pow(10, multiplier++));
    return res;
}

void my_strcpy_ndel(char *dest, char *src)
{
    int i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    delete[] src;
}

void my_strcpy_ndel(std::string &dest, char *src)
{
    int i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    delete[] src;
}

Student lab1::string_to_struct(std::string &java_prop)
{
    Student student;
    student.name = std::string(get_info_after(java_prop, "name"));
    student.group = std::string(get_info_after(java_prop, "group"));
    student.grade = std::stod(get_info_after(java_prop, "grade"));
    return student;
}

Student lab1::string_to_struct(const char *java_prop)
{
    std::string jv_pp = std::string(java_prop);
    return string_to_struct(jv_pp);
}

Student lab1::string_to_struct(const char *java_prop, size_t length)
{
    std::string jv_pp = std::string(java_prop, length);
    return string_to_struct(jv_pp);
}
