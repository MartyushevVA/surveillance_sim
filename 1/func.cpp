#include <prog.h>
using namespace std;
using namespace lab1;

int size_of_string(char string[])
{
    int size = 0;
    while (string[size])
        size++;
    return size;
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
    char *res = new char[size_of_string(str) + 1]{0};
    for (int i = 0; i < size_of_string(str); i++)
        res[i] = str[i];
    delete[] str;
    return res;
}

char *struct_to_string(Student student, const char title[])
{
    int namelen = strlen(student.name);
    int grouplen = strlen(student.group);
    char* grade_string = double_to_string(student.grade);
    int gradelen = strlen(grade_string);
    int titlelen = strlen(title);
    char* java_prop = new char[3 * titlelen + 25]{0};
    strcat(java_prop, title);
    strcat(java_prop, ".name=");
    strcat(java_prop, student.name);
    strcat(java_prop, "\n");
    strcat(java_prop, title);
    strcat(java_prop, ".group=");
    strcat(java_prop, student.group);
    strcat(java_prop, "\n");
    strcat(java_prop, title);
    strcat(java_prop, ".grade=");
    strcat(java_prop, grade_string);
    strcat(java_prop, "\n\0");
    return java_prop;
}

char *get_info_after(const char *src, char *infotype)
{
    static int pos = 0;
    int length = 0;
    while (src[pos] != '=')
        pos++;
    pos++;
    while (src[pos + length] != '\n')
        length++;
    char *data = (char *)calloc(length + 1, sizeof(char));
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
    for (int i = 0; i < strlen(value); i++)
        res = (value[i] - '0') / (simple_power(10, multiplier++));
    return res;
}

Student string_to_struct(const char *java_prop)
{
    Student student;
    char *temp = get_info_after(java_prop, "name");
    student.name = (char *)calloc((strlen(temp) + 1), sizeof(char));
    char *temp = get_info_after(java_prop, "group");
    student.group = (char *)calloc((strlen(temp) + 1), sizeof(char));
    char *temp = get_info_after(java_prop, "grade");
    student.grade = string_to_double(temp);
}