#include "renderer.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <windows.h>
#include <string>
#include <sstream>

namespace SnakeGame {

Renderer::Renderer(const GameConfig& config)
    : config(config), consoleHandle(GetStdHandle(STD_OUTPUT_HANDLE)) {
    // Set up console
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

Renderer::~Renderer() {
    // Restore cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
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

void Renderer::clear() {
    system("cls");
}

void Renderer::refresh() {
    // No need to do anything special for Windows console
}

void Renderer::drawPortal(const Point& position) {
    setTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    drawChar(position.x, position.y, 'O');
}

void Renderer::drawCombo(int combo) {
    if (combo > 1) {
        setTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::stringstream ss;
        ss << "Combo x" << combo << "!";
        drawString(config.width - ss.str().length() - 2, 0, ss.str());
    }
}

void Renderer::drawHardcoreMode() {
    setTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    drawString(2, 1, "HARDCORE MODE");
}

void Renderer::drawGameOver(int finalScore) {
    clear();
    setTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    
    // Draw ASCII art game over
    std::vector<std::string> gameOverArt = {
        "  _____                         ____                 _ ",
        " / ____|                       / __ \\               | |",
        "| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __| |",
        "| | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__| |",
        "| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |  |_|",
        " \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|  (_)"
    };
    
    int startY = (config.height - gameOverArt.size()) / 2;
    for (const auto& line : gameOverArt) {
        drawCenteredText(startY++, line);
    }
    
    // Draw final score
    std::stringstream ss;
    ss << "Final Score: " << finalScore;
    drawCenteredText(startY + 2, ss.str());
    
    // Draw continue message
    setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    drawCenteredText(startY + 4, "Press any key to continue...");
}

void Renderer::drawStartScreen() {
    clear();
    setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    
    // Draw ASCII art title
    std::vector<std::string> titleArt = {
        "  _____ _             _    _____                     ",
        " / ____| |           | |  / ____|                    ",
        "| (___ | | __ _ _ __ | | | |  __  __ _ _ __ ___   ___ ",
        " \\___ \\| |/ _` | '_ \\| | | | |_ |/ _` | '_ ` _ \\ / _ \\",
        " ____) | | (_| | | | |_| | |__| | (_| | | | | | |  __/",
        "|_____/|_|\\__,_|_| |_(_)  \\_____|\\__,_|_| |_| |_|\\___|"
    };
    
    int startY = (config.height - titleArt.size()) / 2;
    for (const auto& line : titleArt) {
        drawCenteredText(startY++, line);
    }
    
    // Draw menu options
    setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    drawCenteredText(startY + 2, "1. Start Game");
    drawCenteredText(startY + 3, "2. Configuration");
    drawCenteredText(startY + 4, "3. Exit");
}

void Renderer::drawConfigScreen() {
    clear();
    setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    drawCenteredText(2, "Configuration");
    drawBox(2, 4, config.width - 4, 10);
    
    int y = 6;
    drawString(4, y++, "Grid Size: " + std::to_string(config.width) + "x" + std::to_string(config.height));
    drawString(4, y++, "Initial Speed: " + std::to_string(config.initialSpeed.count()) + "ms");
    drawString(4, y++, "Wrap Around: " + std::string(config.wrapAround ? "Yes" : "No"));
    drawString(4, y++, "Hardcore Mode: " + std::string(config.hardcoreMode ? "Yes" : "No"));
    
    drawCenteredText(config.height - 2, "Use arrow keys to navigate, ENTER to select");
}

void Renderer::setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(consoleHandle, coord);
}

void Renderer::setTextColor(int color) {
    SetConsoleTextAttribute(consoleHandle, color);
}

void Renderer::drawChar(int x, int y, char c) {
    setCursorPosition(x, y);
    std::cout << c;
}

void Renderer::drawString(int x, int y, const std::string& str) {
    setCursorPosition(x, y);
    std::cout << str;
}

void Renderer::drawBox(int x, int y, int width, int height) {
    // Draw top border
    drawString(x, y, std::string(width, '-'));
    
    // Draw sides
    for (int i = 1; i < height - 1; ++i) {
        drawChar(x, y + i, '|');
        drawChar(x + width - 1, y + i, '|');
    }
    
    // Draw bottom border
    drawString(x, y + height - 1, std::string(width, '-'));
}

void Renderer::drawCenteredText(int y, const std::string& text) {
    int x = (config.width - text.length()) / 2;
    drawString(x, y, text);
}

} // namespace SnakeGame 