#include "dial.h"
using namespace dl;

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