#include "dial.h"
#include "func.h"

int dl::get_option()
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

void dl::dstruct_to_string()
{
    lab1::Student student;
    std::cout << "Введите имя: ";
    std::cin >> student.name;
    std::cout << "Введите группу: ";
    std::cin >> student.group;
    std::cout << "Введите оценку: ";
    std::cin >> student.grade;
    std::string title;
    std::cout << "Введите название структуры: ";
    std::cin >> title;
    std::cout << lab1::struct_to_string(student, title);
}

void dl::dstring_to_struct()
{
    std::string java_prop;
    std::cout << "Введите java prop строку: ";
    getline(std::cin, java_prop);
    lab1::Student student = lab1::string_to_struct(java_prop);
    std::cout << "{name=" << student.name << ", group=" << student.group << ", grade=" << student.grade << "};" << std::endl;
}
