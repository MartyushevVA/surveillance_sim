#include "game.h"

int main() {
    Game game;
    game.loadFieldFromFile("config2.json");
    game.start();
    return 0;
}
