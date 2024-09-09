#include "dial.h"
#include "func.h"

bool check_group(std::string text)
{
    const std::string aproved_letters = "BbSsMmAa";
    if (text.length() != 7)
        return false;
    if (aproved_letters.find(text[0]) == std::string::npos)
        return false;
    if (text[1] < '0' || text[1] > '9')
        return false;
    if (text[2] < '0' || text[2] > '9')
        return false;
    if (text[3] != '-')
        return false;
    if (text[4] < '0' || text[4] > '9')
        return false;
    if (text[5] < '0' || text[5] > '9')
        return false;
    if (text[6] < '0' || text[6] > '9')
        return false;
    return true;
}

bool check_word(std::string text)
{
    std::string *banwords = new std::string[7]{"name", "group", "grade", "=", ".", ",", " "};
    for (int i = 0; i < 7; i++)
        if (text.find(banwords[i]) != std::string::npos)
            return false;
    delete[] banwords;
    return true;
}

bool check_java_format(std::string)
{
    // проверить строку по маске в идеале
    return true;
}

bool check_grade(double num)
{
    return 2 <= num && num <= 5;
}

bool check_option(int)
{
    return true;
}

void s_struct(const lab1::Student student, std::string title)
{
    std::cout << lab1::struct_to_string(student, title);
}

void nt_struct(const lab1::Student student, std::string title)
{
    char *nt = new char[title.length() + 1]{};
    for (std::size_t i = 0; i < title.length(); ++i)
        nt[i] = title[i];
    std::cout << lab1::struct_to_string(student, nt);
    delete[] nt;
}
void ca_struct(const lab1::Student student, std::string title)
{
    char *nt = new char[title.length()];
    for (std::size_t i = 0; i < title.length(); ++i)
        nt[i] = title[i];
    std::cout << lab1::struct_to_string(student, nt, title.length());
    delete[] nt;
}

std::string getString(const std::string &msg, bool checker(std::string))
{
    std::string input;
    std::cout << msg;
    while (true)
    {
        std::cin >> input;
        if (std::cin.eof())
            throw 0;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод, попробуйте снова." << std::endl;
        }
        if (!checker(input))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка формата, повторите ввод." << std::endl;
        }
        else
            return input;
    }
}

std::string getJProp(const std::string &msg, std::function<bool> checker(std::string))
{
    std::string jprop;
    std::string input;
    std::cout << msg;
    while (true)
    {
        std::cin >> input;
        if (!input.length())
        {
            if (!check_java_format(jprop))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                jprop = "";
                std::cout << "Неверный формат, попробуйте снова." << std::endl;
            }
            else
                return jprop;
        }
        if (std::cin.eof())
            throw 0;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            jprop = "";
            std::cout << "Неверный ввод, попробуйте снова." << std::endl;
        }
        else
            jprop += input;
    }
}

double getDouble(const std::string &msg, bool checker(double))
{
    double input;
    std::cout << msg;
    while (true)
    {
        std::cin >> input;
        if (std::cin.eof())
            throw 0;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод, попробуйте снова." << std::endl;
        }
        if (!checker(input))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка формата, повторите ввод." << std::endl;
        }
        else
            return input;
    }
}

int getInt(const std::string &msg, bool checker(int))
{
    int input;
    std::cout << msg;
    while (true)
    {
        std::cin >> input;
        if (std::cin.eof())
            throw 0;
        if (!std::cin.good())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод, попробуйте снова." << std::endl;
        }
        if (!checker(input))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка формата, повторите ввод." << std::endl;
        }
        else
            return input;
    }
}

void d_struct_to_string()
{
    lab1::Student student;
    std::string title;
    title = getString("Введите название структуры (заголовок): ", check_word);
    student.name = getString("Введите имя: ", check_word);
    student.group = getString("Введите группу: ", check_group);
    student.grade = getDouble("Введите оценку: ", check_grade);
    int option;
    while (true)
    {
        try
        {
            option = getInt("Выберите вариант перегрузки функции (конвертация заголовка):\n1) Нуль-терминированная строка\n2) Массив символов и количество\n3) Экземляр класса string\n4) Назад\n", check_option);
            switch (option)
            {
            case 1:
                nt_struct(student, title);
                break;
            case 2:
                ca_struct(student, title);
                break;
            case 3:
                s_struct(student, title);
                break;
            case 4:
                throw " ";
            default:
                throw std::runtime_error("Число вне допустимого диапазона значений. Повторите ввод.");
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
            throw;
        }
    }
}

void s_string(const std::string java_prop)
{
    lab1::Student student = lab1::string_to_struct(java_prop);
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
}

void nt_string(const std::string java_prop)
{
    char *njp = new char[java_prop.length() + 1]{};
    for (std::size_t i = 0; i < java_prop.length(); ++i)
        njp[i] = java_prop[i];
    lab1::Student student = lab1::string_to_struct(njp);
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
    delete[] njp;
}

void ca_string(const std::string java_prop)
{
    char *njp = new char[java_prop.length()]{};
    for (std::size_t i = 0; i < java_prop.length(); ++i)
        njp[i] = java_prop[i];
    lab1::Student student = lab1::string_to_struct(njp, java_prop.length());
    std::cout << "{name=\"" << student.name << "\", group=" << student.group << "\", grade=" << student.grade << "};" << std::endl;
    delete[] njp;
}

void d_string_to_struct()
{
    std::string java_prop;
    java_prop = getJProp("Введите java prop строку:", check_java_format);
    int option = 0;
    while (true)
    {
        try
        {
            option = getInt("Выберите вариант перегрузки функции (конвертация заголовка):\n1) Нуль-терминированная строка\n2) Массив символов и количество\n3) Экземляр класса string\n4) Выход\n", check_option);
            switch (option)
            {
            case 1:
                nt_string(java_prop);
                return;
            case 2:
                ca_string(java_prop);
                return;
            case 3:
                s_string(java_prop);
                return;
            case 4:
                throw 0;
            default:
                throw std::runtime_error("Число вне допустимого диапазона значений. Повторите ввод.");
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (...)
        {
            throw;
        }
    }
}

void dl::dialog()
{
    int option = 0;
    while (true)
    {
        try
        {
            option = getInt("Выберите опцию:\n1) Структура --> java properties\n2) Java properties --> структура\n3) Выход\n", check_option);
            switch (option)
            {
            case 1:
                d_struct_to_string();
                break;
            case 2:
                d_string_to_struct();
                break;
            case 3:
                return;
            default:
                throw std::runtime_error("Число вне допустимого диапазона значений. Повторите ввод.");
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const int)
        {
            return;
        }
        catch (...)
        {
        }
    }
}