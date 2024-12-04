#include <gtest/gtest.h>
#include "game/game.h"

TEST(GameTest, Initialization) {
    EXPECT_NO_THROW(Game game("configs/config2.json", "configs/graphics.json"));
}

TEST(GameTest, UpdateCycle) {
    Game game("configs/config2.json", "configs/graphics.json");
    EXPECT_NO_THROW(game.updateSuspects());
} 