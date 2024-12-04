#include "model/game/game.h"

int main() {
    Game game("configs/config2.json", "configs/graphics.json");
    game.start();
    return 0;
}
