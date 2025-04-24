#pragma once

#include <memory>
#include <chrono>
#include <vector>
#include "constants.h"
#include "snake.h"
#include "food.h"
#include "renderer.h"

namespace SnakeGame {

struct Portal {
    Point position;
    Point destination;
    bool active;
};

class Game {
public:
    Game();
    void run();
    
private:
    GameConfig config;
    std::unique_ptr<Snake> snake;
    std::unique_ptr<Food> food;
    std::unique_ptr<Renderer> renderer;
    
    int score;
    int highScore;
    bool gameOver;
    bool paused;
    std::chrono::milliseconds gameSpeed;
    std::chrono::steady_clock::time_point lastUpdate;
    
    // New features
    std::vector<Portal> portals;
    bool hardcoreMode;
    
    void initialize();
    void handleInput();
    void update();
    void render();
    void loadHighScore();
    void saveHighScore();
    void showStartScreen();
    void showConfigScreen();
    void handleConfigInput();
    void resetGame();
    void checkCollisions();
    void handleFoodEaten();
    void updateSpeed();
    
    // New methods
    void initializePortals();
    void checkPortalCollisions();
    void toggleHardcoreMode();
    void updateHardcoreSpeed();
};

} // namespace SnakeGame 