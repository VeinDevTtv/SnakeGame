#pragma once

#include <string>
#include <vector>
#include <chrono>
#include "constants.h"
#include "snake.h"
#include "food.h"

namespace SnakeGame {

class Renderer {
public:
    Renderer(const GameConfig& config);
    
    void render(const Snake& snake, const Food& food, int score, int highScore, bool paused, bool gameOver);
    void showGameOver(int score, int highScore);
    void showPause();
    void showStartScreen();
    void showConfigScreen(const GameConfig& config);
    
    void animateFoodEaten(const Point& position);
    void animateSnakeDeath(const std::vector<Point>& snakeBody);
    
private:
    GameConfig config;
    std::vector<std::vector<char>> board;
    std::chrono::steady_clock::time_point lastAnimationTime;
    bool isAnimating;
    
    void clearScreen();
    void drawBorder();
    void drawSnake(const Snake& snake);
    void drawFood(const Food& food);
    void drawScore(int score, int highScore);
    void drawControls();
    void centerText(const std::string& text, int y);
    void drawBox(int x, int y, int width, int height);
};

} // namespace SnakeGame 