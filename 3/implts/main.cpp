#include "game.h"

int main() {
    Game game;
    game.loadFieldFromFile("input.json");
    game.start();
    return 0;
}
