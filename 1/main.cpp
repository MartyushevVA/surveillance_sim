#include <prog.h>
using namespace std;
using namespace lab1;

int get_option()
{
    int option = 0;
    while (true)
    {
        cout << "Выберите опцию: " << endl
             << "1) Перевод из структуры в java properties"
             << endl
             << "2) Перевод из java properties в структуру"
             << endl
             << "3) Выход" << endl;
        if ((cin >> option).good() && (option <= 3) && (1 <= option))
            return option;
        if (cin.fail())
        {
            cin.clear();
            cout << "Неверный ввод, повторите" << endl;
        }
        else
            cout << "Число вне допустимого диапазона значений. Повторите ввод" << endl;
        cin.ignore(100, '\n');
    }
}

void dstruct_to_string()
{
    Student student;
    string title;
    cout << "Введите название структуры: ";
    cin >> title;
    cout << "Заполните структуру:" << endl
         << "Имя: ";
    cin >> student.name;
    cout << "Группа: ";
    cin >> student.group;
    cout << "Оценка: ";
    cin >> student.grade;
}

void dstring_to_struct()
{
}

int main()
{
    int option = get_option();
    while (option != 3)
    {
        switch (option)
        {
        case 1:
            dstruct_to_string();
        case 2:
            dstring_to_struct();
        default:
            break;
        }

        option = get_option();
    }
    cout << "Работа завершена" << endl;
}