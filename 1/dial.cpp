#include "dial.h"
#include "func.h"

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
    std::copy(title.begin(), title.end(), nt);
    std::cout << lab1::struct_to_string(student, nt);
    delete[] nt;
}
void ca_struct(const lab1::Student student, std::string title)
{
    char *nt = new char[title.length()];
    std::copy(title.begin(), title.end(), nt);
    std::cout << lab1::struct_to_string(student, nt, title.length());
    delete[] nt;
}

std::string getJProp(const std::string &msg, std::function<bool(std::string)> checker)
{
    std::string jprop;
    std::cout << msg << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true)
    {
        std::string input;
        getline(std::cin, input);
        if (input.empty())
        {
            if (!checker(jprop))
            {
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
            jprop += input + " ";
    }
}

template <class T>
T getData(const std::string &msg, std::function<bool(T)> checker)
{
    T input;
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
        return input;
    }
}

void d_struct_to_string()
{
    lab1::Student student;
    std::string title;
    title = getData<std::string>("Введите название структуры (заголовок): ", lab1::check_word);
    student.name = getData<std::string>("Введите имя: ", lab1::check_word);
    student.group = getData<std::string>("Введите группу: ", lab1::check_group);
    student.grade = getData<double>("Введите оценку: ", lab1::check_grade);
    int option;
    while (true)
    {
        try
        {
            option = getData<int>("Выберите вариант перегрузки функции (конвертация заголовка):\n1) Нуль-терминированная строка\n2) Массив символов и количество\n3) Экземляр класса string\n4) Назад\n", check_option);
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
    std::copy(java_prop.begin(), java_prop.end(), njp);
    lab1::Student student = lab1::string_to_struct(njp);
    std::cout << "{name=\"" << student.name << "\", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
    delete[] njp;
}

void ca_string(const std::string java_prop)
{
    char *njp = new char[java_prop.length()]{};
    std::copy(java_prop.begin(), java_prop.end(), njp);
    lab1::Student student = lab1::string_to_struct(njp, java_prop.length());
    std::cout << "{name=\"" << student.name << "\", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
    delete[] njp;
}

void d_string_to_struct()
{
    std::string java_prop;
    java_prop = getJProp("Введите java prop строку:", lab1::check_java_format);
    int option = 0;
    while (true)
    {
        try
        {
            option = getData<int>("Выберите вариант перегрузки функции (конвертация заголовка):\n1) Нуль-терминированная строка\n2) Массив символов и количество\n3) Экземляр класса string\n4) Выход\n", check_option);
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
            option = getData<int>("Выберите опцию:\n1) Структура --> java properties\n2) Java properties --> структура\n3) Выход\n", check_option);
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