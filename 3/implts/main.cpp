#include "game.h"

int main() {
    Game game;
    game.loadFieldFromFile("config1.json");
    game.start();
    return 0;
}
