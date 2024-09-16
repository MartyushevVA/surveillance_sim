#include "dial.h"
template <class T>

T getData(const std::string &msg, std::function<bool(T)> checker)
{
    T input;
    std::cout << msg;
    while (true)
    {
        std::cin >> input;
        if (std::cin.eof())
            throw std::invalid_argument("Завершение работы.");
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