#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
//#include <vld.h>
#include "dial.h"
using namespace dl;

int main()
{
    dialog();
    std::cout << "Выход из программы." << std::endl;
    _CrtDumpMemoryLeaks();
    return 0;
}