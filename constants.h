#pragma once

#include <string>
#include <chrono>

namespace SnakeGame {

// Game dimensions
constexpr int DEFAULT_WIDTH = 20;
constexpr int DEFAULT_HEIGHT = 20;

// Game characters
constexpr char SNAKE_HEAD = 'O';
constexpr char SNAKE_BODY = 'o';
constexpr char FOOD = '@';
constexpr char EMPTY = '.';
constexpr char WALL = '#';
constexpr char SPEED_FOOD = 'S';
constexpr char REVERSE_FOOD = 'R';

// Timing
constexpr auto INITIAL_SPEED = std::chrono::milliseconds(100);
constexpr auto MIN_SPEED = std::chrono::milliseconds(50);
constexpr auto SPEED_INCREMENT = std::chrono::milliseconds(10);

// Game modes
enum class GameMode {
    CLASSIC,    // With walls
    WRAP_AROUND // Snake wraps around edges
};

// Difficulty levels
enum class Difficulty {
    EASY,
    NORMAL,
    HARD
};

// Food types
enum class FoodType {
    NORMAL,
    SPEED_BOOST,
    REVERSE_CONTROLS
};

// Configuration structure
struct GameConfig {
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    GameMode mode = GameMode::CLASSIC;
    Difficulty difficulty = Difficulty::NORMAL;
    bool enableSpecialFood = true;
    bool enableAnimations = true;
};

} // namespace SnakeGame 