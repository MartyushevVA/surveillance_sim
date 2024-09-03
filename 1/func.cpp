#include <prog.h>
using namespace std;
using namespace lab1;

int size_of_char(const char *string)
{
    int size = 0;
    while (string[size])
        size++;
    return size;
}

void chars_concat(char *dest, const char *src)
{
    int pos = 0, i = 0;
    while (dest[pos])
        pos++;
    while (src[i])
        dest[pos + i] = src[i++];
    dest[pos + i] = 0;
}

char *double_to_string(double value)
{
    char *str = new char[10]{0};
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
    char *res = new char[size_of_char(str) + 1]{0};
    for (int i = 0; i < size_of_char(str); i++)
        res[i] = str[i];
    delete[] str;
    return res;
}

char *struct_to_string(Student student, const char *title)
{
    int namelen = size_of_char(student.name);
    int grouplen = size_of_char(student.group);
    char *grade_string = double_to_string(student.grade);
    int gradelen = size_of_char(grade_string);
    int titlelen = size_of_char(title);
    char *java_prop = new char[3 * titlelen + 25]{0};
    chars_concat(java_prop, title);
    chars_concat(java_prop, ".name=");
    chars_concat(java_prop, student.name);
    chars_concat(java_prop, "\n");
    chars_concat(java_prop, title);
    chars_concat(java_prop, ".group=");
    chars_concat(java_prop, student.group);
    chars_concat(java_prop, "\n");
    chars_concat(java_prop, title);
    chars_concat(java_prop, ".grade=");
    chars_concat(java_prop, grade_string);
    chars_concat(java_prop, "\n\0");
    return java_prop;
}

void chars_concat(char *dest, const char *src, size_t src_size)
{
    int pos = 0, i = 0;
    while (dest[pos])
        pos++;
    for (int i = 0; i < src_size; i++)
        dest[pos + i] = src[i++];
    dest[pos + i] = 0;
}

char *struct_to_string(Student student, const char *title, size_t len)
{
    int namelen = size_of_char(student.name);
    int grouplen = size_of_char(student.group);
    char *grade_string = double_to_string(student.grade);
    int gradelen = size_of_char(grade_string);
    char *java_prop = new char[3 * len + 25]{0};
    chars_concat(java_prop, title, len);
    chars_concat(java_prop, ".name=");
    chars_concat(java_prop, student.name);
    chars_concat(java_prop, "\n");
    chars_concat(java_prop, title, len);
    chars_concat(java_prop, ".group=");
    chars_concat(java_prop, student.group);
    chars_concat(java_prop, "\n");
    chars_concat(java_prop, title, len);
    chars_concat(java_prop, ".grade=");
    chars_concat(java_prop, grade_string);
    chars_concat(java_prop, "\n\0");
    return java_prop;
}

string struct_to_string(Student student, string title)
{
    string name = string(student.name);
    string group = string(student.group);
    string grade = string(double_to_string(student.grade));
    string java_prop = title + ".name=" + name + "\n" + title + ".group=" + group + "\n" + title + ".grade=" + grade + "\n";
    return java_prop;
}

char *get_info_after(const char *src, char *infotype, size_t len = 0)
{
    static int pos = 0;
    int length = 0;
    while (src[pos] != '=')
        pos++;
    pos++;
    while (src[pos + length] != '\n')
        length++;
    char *data = new char[length + 1]{0};
    for (int i = 0; i < length; i++)
        data[i] = src[pos + i];
    return data;
}

int simple_power(int base, int pow)
{
    if (!pow)
        return 1;
    for (pow; pow > 0; pow--)
        base *= base;
    return base;
}

double string_to_double(char *value)
{
    int multiplier = 0;
    double res = value[0] - '0';
    for (int i = 0; i < size_of_char(value); i++)
        res = (value[i] - '0') / (simple_power(10, multiplier++));
    return res;
}

Student string_to_struct(const char *java_prop)
{
    Student student;
    char *temp = get_info_after(java_prop, "name");
    student.name = new char[size_of_char(temp) + 1]{0};
    char *temp = get_info_after(java_prop, "group");
    student.group = new char[size_of_char(temp) + 1]{0};
    char *temp = get_info_after(java_prop, "grade");
    student.grade = string_to_double(temp);
}
Student string_to_struct(const char *java_prop, size_t len)
{
    Student student;
    char *temp = get_info_after(java_prop, "name", len);
    student.name = new char[size_of_char(temp) + 1]{0};
    char *temp = get_info_after(java_prop, "group", len);
    student.group = new char[size_of_char(temp) + 1]{0};
    char *temp = get_info_after(java_prop, "grade", len);
    student.grade = string_to_double(temp);
}
Student string_to_struct(string java_prop)
{
    Student student;
    char *temp = get_info_after(java_prop, "name");
    student.name = new char[size_of_char(temp) + 1]{0};
    char *temp = get_info_after(java_prop, "group");
    student.group = new char[size_of_char(temp) + 1]{0};
    char *temp = get_info_after(java_prop, "grade");
    student.grade = string_to_double(temp);
}