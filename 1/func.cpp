#include "func.h"
using namespace lab1;

bool lab1::check_group(const std::string &text)
{
    const std::string aproved_letters = "BbSsMmAa";
    const size_t masked_nums[5]{1, 2, 4, 5, 6};
    if (text.length() != 7 || aproved_letters.find(text[0]) == std::string::npos)
        return false;
    if (std::any_of(std::begin(masked_nums), std::end(masked_nums), [text](size_t pos)
                    { return text[pos] < '0' || text[pos] > '9'; }))
        return false;
    if (text[3] != '-')
        return false;
    return true;
}

bool lab1::check_word(const std::string &text)
{
    const std::string banwords[7]{"name", "group", "grade", "=", ".", ",", " "};
    if (std::any_of(std::begin(banwords), std::end(banwords), [text](const std::string &word)
                    { return text.find(word) != std::string::npos; }))
        return false;
    return true;
}

bool lab1::check_java_format(const std::string &)
{
    // проверить строку по маске в идеале
    return true;
}

bool lab1::check_grade(double num)
{
    return 2 <= num && num <= 5;
}

std::string lab1::struct_to_string(const Student &student, const std::string &title)
{
    bool checks[4]{check_word(title), check_word(student.name), check_group(student.group), check_grade(student.grade)};
    try
    {
        if (std::find(checks, checks + 4, false) != checks + 4)
            throw std::runtime_error("Ошибка формата.");
        std::string java_prop = title + ".name=" + student.name + "\n" + title + ".group=" + student.group + "\n" + title + ".grade=" + std::to_string(student.grade) + "\n";
        return java_prop;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
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
    if (!check_java_format(src))
        throw std::runtime_error("Ошибка формата.");
    size_t base = src.find(infotype);
    size_t start = src.find('=', base) + 1;
    size_t compos = src.find(' ', start), nullpos = src.find('\0', start);
    size_t end = std::min(compos, nullpos);
    return src.substr(start, end - start);
}

double string_to_double(const std::string &value)
{
    double multiplier = 0.1;
    double res = value[0] - '0';
    std::for_each(value.begin() + 2, value.end(), [&](char symb)
                  {res+=(symb-'0')*multiplier; multiplier+=0.1; });
    return res;
}

Student lab1::string_to_struct(const std::string &java_prop)
{
    try
    {
        Student student = {get_info_after(java_prop, "name"), get_info_after(java_prop, "group"), string_to_double(get_info_after(java_prop, "grade"))};
        return student;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
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
