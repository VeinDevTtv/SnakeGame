#pragma once

#include <string>
#include <chrono>

namespace SnakeGame {

// Console colors
constexpr int FOREGROUND_BLUE = 0x0001;
constexpr int FOREGROUND_GREEN = 0x0002;
constexpr int FOREGROUND_RED = 0x0004;
constexpr int FOREGROUND_INTENSITY = 0x0008;
constexpr int BACKGROUND_BLUE = 0x0010;
constexpr int BACKGROUND_GREEN = 0x0020;
constexpr int BACKGROUND_RED = 0x0040;
constexpr int BACKGROUND_INTENSITY = 0x0080;

// Game dimensions
constexpr int DEFAULT_WIDTH = 20;
constexpr int DEFAULT_HEIGHT = 20;

// Game characters
constexpr char SNAKE_HEAD = '@';
constexpr char SNAKE_BODY = 'o';
constexpr char FOOD = '*';
constexpr char PORTAL = 'O';
constexpr char EMPTY = ' ';
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
    int width;
    int height;
    std::chrono::milliseconds initialSpeed;
    bool wrapAround;
    bool hardcoreMode;
    bool enableAnimations;
    
    static GameConfig defaultConfig() {
        return {
            40,     // width
            20,     // height
            std::chrono::milliseconds(200),  // initialSpeed
            true,   // wrapAround
            false,  // hardcoreMode
            true    // enableAnimations
        };
    }
};

// Game states
enum class GameState {
    START_SCREEN,
    CONFIG_SCREEN,
    PLAYING,
    PAUSED,
    GAME_OVER
};

// Input handling
enum class InputAction {
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    PAUSE,
    QUIT,
    CONFIRM,
    BACK
};

} // namespace SnakeGame 