#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <deque>
#include "constants.h"
#include "snake.h"
#include "food.h"
#include "point.h"

namespace SnakeGame {

struct HighScoreEntry {
    std::string name;
    int score;
    std::chrono::system_clock::time_point date;
};

class Renderer {
public:
    Renderer(const GameConfig& config);
    ~Renderer();
    
    void clear();
    void refresh();
    
    // Drawing methods
    void drawSnake(const std::deque<Point>& body);
    void drawFood(const Point& position);
    void drawPortal(const Point& position);
    void drawScore(int score, int highScore);
    void drawCombo(int combo);
    void drawHardcoreMode();
    void drawGameOver(int finalScore);
    void drawStartScreen();
    void drawConfigScreen(const GameConfig& config);
    void drawString(int x, int y, const std::string& str);
    void drawChar(int x, int y, char c);
    void drawBox(int x, int y, int width, int height);
    void drawCenteredText(int y, const std::string& text);
    
    // Animation methods
    void animateFoodEaten(const Point& position);
    void animateSnakeDeath(const std::deque<Point>& snakeBody);
    void animateScoreCountUp(int finalScore, int startScore = 0);
    void animateBounceText(int y, const std::string& text, int duration);
    void animateWaveText(int y, const std::string& text, int duration);
    
    // Mode settings
    void setMinimalMode(bool enabled) { minimalMode = enabled; }
    
    void drawHighScoreTable(const std::vector<HighScoreEntry>& highScores);
    
private:
    GameConfig config;
    std::vector<std::vector<char>> board;
    std::chrono::steady_clock::time_point lastAnimationTime;
    bool isAnimating;
    bool minimalMode;
    void* consoleHandle;
    
    void setCursorPosition(int x, int y);
    void setTextColor(int color);
    void drawBorder();
    void drawSnake(const Snake& snake);
    void drawFood(const Food& food);
    void drawControls();
    void centerText(const std::string& text, int y);
};

} // namespace SnakeGame 