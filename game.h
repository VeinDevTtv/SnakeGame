#pragma once

#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include "constants.h"
#include "snake.h"
#include "food.h"
#include "renderer.h"
#include "replay.h"
#include "achievements.h"

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
    std::unique_ptr<ReplaySystem> replaySystem;
    std::unique_ptr<AchievementSystem> achievementSystem;
    
    int score;
    int highScore;
    bool gameOver;
    bool paused;
    std::chrono::milliseconds gameSpeed;
    std::chrono::steady_clock::time_point lastUpdate;
    std::chrono::steady_clock::time_point gameStartTime;
    
    // New features
    std::vector<Portal> portals;
    bool hardcoreMode;
    bool minimalMode;
    int portalUseCount;
    
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
    void toggleMinimalMode();
    
    // Replay methods
    void startReplayRecording();
    void stopReplayRecording();
    void showReplayMenu();
    void playReplay(const std::string& filename);
    
    // Achievement methods
    void updateAchievements();
    void showAchievements();
    
    // Game state
    GameState currentState;
    std::string playerName;
};

} // namespace SnakeGame 