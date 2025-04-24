#include "game.h"
#include <conio.h>
#include <fstream>
#include <thread>

namespace SnakeGame {

Game::Game()
    : score(0), highScore(0), gameOver(false), paused(false),
      gameSpeed(INITIAL_SPEED) {
    initialize();
}

void Game::run() {
    showStartScreen();
    
    while (!gameOver) {
        handleInput();
        
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
        
        if (elapsed >= gameSpeed && !paused) {
            update();
            lastUpdate = now;
        }
        
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::initialize() {
    loadHighScore();
    resetGame();
}

void Game::handleInput() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w':
            case 'W':
            case 's':
            case 'S':
            case 'a':
            case 'A':
            case 'd':
            case 'D':
                if (!paused) {
                    snake->move(DirectionManager::fromChar(key), config);
                }
                break;
            case 'p':
            case 'P':
                paused = !paused;
                break;
            case 27: // ESC
                gameOver = true;
                break;
        }
    }
}

void Game::update() {
    if (paused || gameOver) return;
    
    checkCollisions();
    if (gameOver) return;
    
    handleFoodEaten();
    updateSpeed();
}

void Game::render() {
    renderer->render(*snake, *food, score, highScore, paused, gameOver);
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void Game::saveHighScore() {
    if (score > highScore) {
        highScore = score;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }
}

void Game::showStartScreen() {
    renderer->showStartScreen();
    while (true) {
        char key = _getch();
        if (key == 13) break; // Enter
        if (key == 'c' || key == 'C') {
            showConfigScreen();
            break;
        }
        if (key == 27) { // ESC
            gameOver = true;
            break;
        }
    }
}

void Game::showConfigScreen() {
    renderer->showConfigScreen(config);
    handleConfigInput();
}

void Game::handleConfigInput() {
    while (true) {
        char key = _getch();
        if (key == 13) break; // Enter
        
        switch (key) {
            case '1':
                config.width = (config.width == 20) ? 30 : 20;
                config.height = (config.height == 20) ? 30 : 20;
                break;
            case '2':
                config.mode = (config.mode == GameMode::CLASSIC) ? 
                             GameMode::WRAP_AROUND : GameMode::CLASSIC;
                break;
            case '3':
                config.difficulty = static_cast<Difficulty>(
                    (static_cast<int>(config.difficulty) + 1) % 3);
                break;
            case '4':
                config.enableSpecialFood = !config.enableSpecialFood;
                break;
            case '5':
                config.enableAnimations = !config.enableAnimations;
                break;
        }
        renderer->showConfigScreen(config);
    }
}

void Game::resetGame() {
    snake = std::make_unique<Snake>(config.width / 2, config.height / 2);
    food = std::make_unique<Food>(config);
    renderer = std::make_unique<Renderer>(config);
    
    score = 0;
    gameOver = false;
    paused = false;
    gameSpeed = INITIAL_SPEED;
    lastUpdate = std::chrono::steady_clock::now();
    
    food->place(snake->getBody(), config);
}

void Game::checkCollisions() {
    if (snake->checkSelfCollision() || 
        (config.mode == GameMode::CLASSIC && snake->checkWallCollision(config))) {
        gameOver = true;
        if (config.enableAnimations) {
            renderer->animateSnakeDeath(snake->getBody());
        }
        saveHighScore();
    }
}

void Game::handleFoodEaten() {
    if (snake->getHead() == food->getPosition()) {
        score++;
        snake->grow();
        
        if (config.enableAnimations) {
            renderer->animateFoodEaten(food->getPosition());
        }
        
        food->place(snake->getBody(), config);
        
        // Handle special food effects
        if (food->isSpecial()) {
            // TODO: Implement special food effects
        }
    }
}

void Game::updateSpeed() {
    if (score % 5 == 0) {
        gameSpeed = std::max(MIN_SPEED, gameSpeed - SPEED_INCREMENT);
    }
}

} // namespace SnakeGame 