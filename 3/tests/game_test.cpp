#include <gtest/gtest.h>
#include "game/game.h"

TEST(GameTest, Initialization) {
    EXPECT_NO_THROW(Game game("test_field.json", "test_graphics.json"));
}

TEST(GameTest, UpdateCycle) {
    Game game("test_field.json", "test_graphics.json");
    // Test one update cycle
    EXPECT_NO_THROW(game.updateSuspects());
} 