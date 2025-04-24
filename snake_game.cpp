#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <string>

// Game constants
const int WIDTH = 20;
const int HEIGHT = 20;
const int INITIAL_SPEED = 100; // milliseconds
const char SNAKE_HEAD = 'O';
const char SNAKE_BODY = 'o';
const char FOOD = '@';
const char EMPTY = '.';
const char WALL = '#';

// Game state
struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class SnakeGame {
private:
    std::vector<Point> snake;
    Point food;
    int score;
    int highScore;
    bool gameOver;
    bool paused;
    int speed;
    char direction;
    std::vector<std::vector<char>> board;

    void initializeBoard() {
        board.resize(HEIGHT, std::vector<char>(WIDTH, EMPTY));
        // Add walls
        for (int i = 0; i < WIDTH; i++) {
            board[0][i] = WALL;
            board[HEIGHT - 1][i] = WALL;
        }
        for (int i = 0; i < HEIGHT; i++) {
            board[i][0] = WALL;
            board[i][WIDTH - 1] = WALL;
        }
    }

    void placeFood() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(1, WIDTH - 2);
        std::uniform_int_distribution<> disY(1, HEIGHT - 2);

        do {
            food.x = disX(gen);
            food.y = disY(gen);
        } while (std::find(snake.begin(), snake.end(), food) != snake.end());
    }

    void loadHighScore() {
        std::ifstream file("highscore.txt");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0;
        }
    }

    void saveHighScore() {
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }

public:
    SnakeGame() : score(0), gameOver(false), paused(false), speed(INITIAL_SPEED), direction('d') {
        // Initialize snake
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        snake.push_back({WIDTH / 2 - 1, HEIGHT / 2});
        snake.push_back({WIDTH / 2 - 2, HEIGHT / 2});

        initializeBoard();
        placeFood();
        loadHighScore();
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'w':
                case 'W':
                    if (direction != 's') direction = 'w';
                    break;
                case 's':
                case 'S':
                    if (direction != 'w') direction = 's';
                    break;
                case 'a':
                case 'A':
                    if (direction != 'd') direction = 'a';
                    break;
                case 'd':
                case 'D':
                    if (direction != 'a') direction = 'd';
                    break;
                case 'p':
                case 'P':
                    paused = !paused;
                    break;
                case 27: // ESC key
                    gameOver = true;
                    break;
            }
        }
    }

    void update() {
        if (paused || gameOver) return;

        Point newHead = snake.front();
        switch (direction) {
            case 'w': newHead.y--; break;
            case 's': newHead.y++; break;
            case 'a': newHead.x--; break;
            case 'd': newHead.x++; break;
        }

        // Check for collisions
        if (newHead.x <= 0 || newHead.x >= WIDTH - 1 ||
            newHead.y <= 0 || newHead.y >= HEIGHT - 1 ||
            std::find(snake.begin(), snake.end(), newHead) != snake.end()) {
            gameOver = true;
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
            return;
        }

        // Move snake
        snake.insert(snake.begin(), newHead);

        // Check if food is eaten
        if (newHead == food) {
            score++;
            placeFood();
            // Increase speed every 5 points
            if (score % 5 == 0) {
                speed = std::max(50, speed - 10);
            }
        } else {
            snake.pop_back();
        }
    }

    void render() {
        system("cls"); // Clear screen

        // Update board
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                board[i][j] = EMPTY;
            }
        }

        // Draw walls
        for (int i = 0; i < WIDTH; i++) {
            board[0][i] = WALL;
            board[HEIGHT - 1][i] = WALL;
        }
        for (int i = 0; i < HEIGHT; i++) {
            board[i][0] = WALL;
            board[i][WIDTH - 1] = WALL;
        }

        // Draw snake
        for (size_t i = 0; i < snake.size(); i++) {
            board[snake[i].y][snake[i].x] = (i == 0) ? SNAKE_HEAD : SNAKE_BODY;
        }

        // Draw food
        board[food.y][food.x] = FOOD;

        // Print board
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                std::cout << board[i][j] << ' ';
            }
            std::cout << '\n';
        }

        // Print score and controls
        std::cout << "\nScore: " << score << " | High Score: " << highScore << '\n';
        std::cout << "Controls: WASD to move, P to pause, ESC to quit\n";
        if (paused) std::cout << "PAUSED\n";
        if (gameOver) std::cout << "GAME OVER! Press any key to exit.\n";
    }

    bool isGameOver() const { return gameOver; }
    bool isPaused() const { return paused; }
    int getSpeed() const { return speed; }
};

int main() {
    // Hide cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    SnakeGame game;
    while (!game.isGameOver()) {
        game.handleInput();
        game.update();
        game.render();
        Sleep(game.getSpeed());
    }

    // Wait for key press before exiting
    _getch();
    return 0;
} 