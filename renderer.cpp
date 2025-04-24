#include "renderer.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <windows.h>

namespace SnakeGame {

Renderer::Renderer(const GameConfig& config)
    : config(config), isAnimating(false) {
    board.resize(config.height, std::vector<char>(config.width, EMPTY));
}

void Renderer::render(const Snake& snake, const Food& food, int score, int highScore, bool paused, bool gameOver) {
    clearScreen();
    
    // Reset board
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), EMPTY);
    }
    
    drawBorder();
    drawSnake(snake);
    drawFood(food);
    drawScore(score, highScore);
    drawControls();
    
    if (paused) showPause();
    if (gameOver) showGameOver(score, highScore);
}

void Renderer::showGameOver(int score, int highScore) {
    int centerY = config.height / 2;
    centerText("GAME OVER", centerY - 2);
    centerText("Score: " + std::to_string(score), centerY);
    centerText("High Score: " + std::to_string(highScore), centerY + 1);
    centerText("Press any key to exit", centerY + 3);
}

void Renderer::showPause() {
    int centerY = config.height / 2;
    centerText("PAUSED", centerY);
    centerText("Press P to resume", centerY + 1);
}

void Renderer::showStartScreen() {
    clearScreen();
    int centerY = config.height / 2;
    
    centerText("SNAKE GAME", centerY - 3);
    centerText("Press ENTER to start", centerY);
    centerText("Press C for configuration", centerY + 1);
    centerText("Press ESC to quit", centerY + 2);
}

void Renderer::showConfigScreen(const GameConfig& config) {
    clearScreen();
    int centerY = config.height / 2;
    
    centerText("CONFIGURATION", centerY - 4);
    centerText("1. Grid Size: " + std::to_string(config.width) + "x" + std::to_string(config.height), centerY - 2);
    centerText("2. Game Mode: " + (config.mode == GameMode::CLASSIC ? "Classic" : "Wrap Around"), centerY - 1);
    centerText("3. Difficulty: " + std::to_string(static_cast<int>(config.difficulty)), centerY);
    centerText("4. Special Food: " + (config.enableSpecialFood ? "On" : "Off"), centerY + 1);
    centerText("5. Animations: " + (config.enableAnimations ? "On" : "Off"), centerY + 2);
    centerText("Press number to change, ENTER to start", centerY + 4);
}

void Renderer::animateFoodEaten(const Point& position) {
    if (!config.enableAnimations) return;
    
    isAnimating = true;
    lastAnimationTime = std::chrono::steady_clock::now();
    
    // Flash effect
    for (int i = 0; i < 3; ++i) {
        board[position.y][position.x] = '*';
        render(Snake(0, 0), Food(config), 0, 0, false, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        board[position.y][position.x] = EMPTY;
        render(Snake(0, 0), Food(config), 0, 0, false, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    isAnimating = false;
}

void Renderer::animateSnakeDeath(const std::vector<Point>& snakeBody) {
    if (!config.enableAnimations) return;
    
    isAnimating = true;
    lastAnimationTime = std::chrono::steady_clock::now();
    
    // Death animation
    for (const auto& point : snakeBody) {
        board[point.y][point.x] = 'X';
        render(Snake(0, 0), Food(config), 0, 0, false, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    isAnimating = false;
}

void Renderer::clearScreen() {
    system("cls");
}

void Renderer::drawBorder() {
    for (int i = 0; i < config.width; i++) {
        board[0][i] = WALL;
        board[config.height - 1][i] = WALL;
    }
    for (int i = 0; i < config.height; i++) {
        board[i][0] = WALL;
        board[i][config.width - 1] = WALL;
    }
}

void Renderer::drawSnake(const Snake& snake) {
    const auto& body = snake.getBody();
    for (size_t i = 0; i < body.size(); ++i) {
        const auto& point = body[i];
        board[point.y][point.x] = (i == 0) ? SNAKE_HEAD : SNAKE_BODY;
    }
}

void Renderer::drawFood(const Food& food) {
    const auto& pos = food.getPosition();
    board[pos.y][pos.x] = food.getDisplayChar();
}

void Renderer::drawScore(int score, int highScore) {
    std::cout << "\nScore: " << score << " | High Score: " << highScore << '\n';
}

void Renderer::drawControls() {
    std::cout << "Controls: WASD to move, P to pause, ESC to quit\n";
}

void Renderer::centerText(const std::string& text, int y) {
    int x = (config.width - text.length()) / 2;
    std::cout << std::string(x, ' ') << text << '\n';
}

void Renderer::drawBox(int x, int y, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                board[y + i][x + j] = WALL;
            }
        }
    }
}

} // namespace SnakeGame 