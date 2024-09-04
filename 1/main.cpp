#include "prog.h"
using namespace lab1;

int get_option()
{
    int option = 0;
    while (true)
    {
        std::cout << "Выберите опцию: " << std::endl
                  << "1) Перевод из структуры в java properties"
                  << std::endl
                  << "2) Перевод из java properties в структуру"
                  << std::endl
                  << "3) Выход" << std::endl;
        if ((std::cin >> option).good() && (option <= 3) && (1 <= option))
            return option;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cout << "Неверный ввод, повторите" << std::endl;
        }
        else
            std::cout << "Число вне допустимого диапазона значений. Повторите ввод" << std::endl;
        std::cin.ignore(100, '\n');
    }
}

void dstruct_to_string()
{
    Student student;
    std::cout << "Введите имя: ";
    std::cin >> student.name;
    std::cout << "Введите группу: ";
    std::cin >> student.group;
    std::cout << "Введите оценку: ";
    std::cin >> student.grade;
    std::string title;
    std::cout << "Введите название структуры: ";
    std::cin >> title;
    std::cout << struct_to_string(student, title);
}

void dstring_to_struct()
{
    std::string java_prop;
    std::cout << "Введите java prop строку: ";
    getline(std::cin, java_prop);
    Student student = string_to_struct(java_prop);
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
}

int main()
{
    int option = get_option();
    while (option != 3)
    {
        std::cin.ignore(100, '\n');
        switch (option)
        {
        case 1:
            dstruct_to_string();
            break;
        case 2:
            dstring_to_struct();
            break;
        default:
            break;
        }
        option = get_option();
    }
    std::cout << "Работа завершена" << std::endl;
}