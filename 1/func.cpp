#include "func.h"
using namespace lab1;

std::string lab1::struct_to_string(const Student &student, const std::string &title)
{
    std::string java_prop = title + ".name=" + student.name + "\n" + title + ".group=" + student.group + "\n" + title + ".grade=" + std::to_string(student.grade) + "\n";
    return java_prop;
}

std::string lab1::struct_to_string(const Student &student, const char *title)
{
    const std::string &ttl = std::string(title);
    return struct_to_string(student, ttl);
}

std::string lab1::struct_to_string(const Student &student, const char *title, size_t length)
{
    const std::string &ttl = std::string(title, length);
    return struct_to_string(student, ttl);
}

std::string get_info_after(const std::string &src, const char *infotype)
{
    size_t base = src.find(infotype);
    size_t start = src.find('=', base) + 1;
    size_t compos = src.find(',', start), nullpos = src.find('\0', start);
    size_t end = std::min(compos, nullpos);
    return src.substr(start, end-start);
}

double string_to_double(const std::string &value)
{
    double multiplier = 0.1;
    double res = value[0] - '0';
    for (size_t i = 2; i < value.length(); i++)
    {
        res += (value[i] - '0') * multiplier;
        multiplier *= 0.1;
    }
    return res;
}

Student lab1::string_to_struct(const std::string &java_prop)
{
    Student student = {get_info_after(java_prop, "name"), get_info_after(java_prop, "group"), string_to_double(get_info_after(java_prop, "grade"))};
    return student;
}

Student lab1::string_to_struct(const char *java_prop)
{
    const std::string &jv_pp = std::string(java_prop);
    return string_to_struct(jv_pp);
}

Student lab1::string_to_struct(const char *java_prop, size_t length)
{
    const std::string &jv_pp = std::string(java_prop, length);
    return string_to_struct(jv_pp);
}
