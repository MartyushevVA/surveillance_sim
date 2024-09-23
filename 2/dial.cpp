#include "dial.h"
#include "sclass.h"
#include "hclass.h"
#include "alllibs.h"
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
            throw std::invalid_argument("Завершение работы.");
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

void dsimple()
{
    int opt;
    std::cout << "Выберите опцию: ";
    std::cin >> opt;
    static task arr[2] = {};
    switch (opt)
    {
    case 0: // инициализация
    {
        std::cout << "Заполните 1-ю работу: ";
        std::cin >> arr[0];
        std::cout << "Введено: " << arr[0] << std::endl;
        std::cout << "Заполните 2-ю работу: ";
        std::cin >> arr[1];
        std::cout << "Введено: " << arr[1] << std::endl;
        break;
    }
    case 1: // сложение работ
    {
        std::cout << "Результат сложения: " << arr[0] + arr[1] << std::endl;
        break;
    }
    case 2: // разбиение
    {
        task *sheets = arr[0].fragmentation();
        std::cout << "Результат разбиения:" << std::endl;
        for (size_t ind = 0; (int)ind < arr[0].getNumOfSheets(); ind++)
            std::cout << sheets[ind] << std::endl;
        break;
    }
    case 3: // сравнение
    {
        std::cout << "Результат сравнения: " << (arr[0] < arr[1]) << std::endl;
        break;
    }
    case 4: // выставление оценки
    {
        int grade;
        std::cout << "Введите оценку: ";
        std::cin >> grade;
        arr[0].evaluate(grade);
        std::cout << "Результат оценивания: " << arr[0] << std::endl;
        break;
    }
    default:
        return;
    }
}

void dhard()
{
    int opt;
    std::cout << "Выберите опцию: ";
    std::cin >> opt;
    static stack arr = stack();
    switch (opt)
    {
    case 0: // инициализация
    {
        std::cout << "Заполните стек: ";
        std::cin >> arr;
        std::cout << "Введено: " << arr << std::endl;
        break;
    }
    case 1: // добавление работы
    {
        task elem = task();
        std::cout << "Введите добавляемую работу: ";
        std::cin >> elem;
        std::cout << "Результат: " << (arr += elem) << std::endl;
        break;
    }
    case 2: // извлечение одной работы с вершины стопки
    {
        task elem = arr.pop();
        std::cout << "Извлеченная работа: " << elem << std::endl;
        std::cout << "Стек: " << arr << std::endl;
        break;
    }
    case 3: // проверка состояния стека
    {
        std::cout << "Результат проверки: " << arr.fullness() << std::endl;
        break;
    }
    case 4: // объединение всех разбитых на части работ (то есть после выполнения операции для
            // каждого студента должно остаться только по одной работе, если это возможно)
    {
        arr.unioning();
        std::cout << "Результат объединения работ: " << arr << std::endl;
        break;
    }
    case 5: // разбиение всех работ в стопке на отдельные листы
    {
        arr.fragmentation();
        std::cout << "Результат разбиения работ: " << arr << std::endl;
        break;
    }
    case 6: // извлечение следующей работы без оценки
    {
        task elem = arr.extractNextUngraded();
        std::cout << "Извлеченная работа: " << elem << std::endl;
        std::cout << "Стек: " << arr << std::endl;
        break;
    }
    default:
        return;
    }
}

void dialog()
{
    int opt;
    while (true)
    {
        std::cout << "Выберите опцию: ";
        std::cin >> opt;
        switch (opt)
        {
        case 0: // работа с простым классом
            dsimple();
            break;
        case 1: // работа со сложным классом
            dhard();
            break;
        default:
            return;
        }
    }
}