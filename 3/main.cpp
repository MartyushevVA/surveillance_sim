#include "model/controller/controller.h"

int main() {
    Controller controller("configs/config2.json", "configs/graphics.json");
    controller.start();
    return 0;
}
