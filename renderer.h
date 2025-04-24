#pragma once

#include <string>
#include <vector>
#include <chrono>
#include "constants.h"
#include "snake.h"
#include "food.h"
#include "point.h"

namespace SnakeGame {

class Renderer {
public:
    Renderer(const GameConfig& config);
    ~Renderer();
    
    void clear();
    void refresh();
    
    void drawSnake(const std::deque<Point>& body);
    void drawFood(const Point& position);
    void drawPortal(const Point& position);
    void drawScore(int score, int highScore);
    void drawCombo(int combo);
    void drawHardcoreMode();
    void drawGameOver(int finalScore);
    void drawStartScreen();
    void drawConfigScreen();
    
    void animateFoodEaten(const Point& position);
    void animateSnakeDeath(const std::vector<Point>& snakeBody);
    
private:
    GameConfig config;
    std::vector<std::vector<char>> board;
    std::chrono::steady_clock::time_point lastAnimationTime;
    bool isAnimating;
    void* consoleHandle;
    
    void setCursorPosition(int x, int y);
    void setTextColor(int color);
    void drawChar(int x, int y, char c);
    void drawString(int x, int y, const std::string& str);
    void drawBox(int x, int y, int width, int height);
    void drawCenteredText(int y, const std::string& text);
    
    void clearScreen();
    void drawBorder();
    void drawSnake(const Snake& snake);
    void drawFood(const Food& food);
    void drawScore(int score, int highScore);
    void drawControls();
    void centerText(const std::string& text, int y);
};

} // namespace SnakeGame 