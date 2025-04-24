#include "game.h"
#include <conio.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>

namespace SnakeGame {

Game::Game()
    : score(0), highScore(0), gameOver(false), paused(false),
      gameSpeed(std::chrono::milliseconds(200)), hardcoreMode(false) {
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
    config = GameConfig::defaultConfig();
    snake = std::make_unique<Snake>(config.width / 2, config.height / 2);
    food = std::make_unique<Food>(config);
    renderer = std::make_unique<Renderer>(config);
    loadHighScore();
    initializePortals();
}

void Game::initializePortals() {
    // Create two portals at opposite corners
    portals.clear();
    portals.push_back({Point(1, 1), Point(config.width - 2, config.height - 2), true});
    portals.push_back({Point(config.width - 2, config.height - 2), Point(1, 1), true});
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
    if (gameOver || paused) return;
    
    auto now = std::chrono::steady_clock::now();
    if (now - lastUpdate < gameSpeed) return;
    
    lastUpdate = now;
    
    snake->move(snake->getCurrentDirection(), config);
    checkCollisions();
    checkPortalCollisions();
    
    if (snake->getHead() == food->getPosition()) {
        handleFoodEaten();
    }
}

void Game::render() {
    renderer->clear();
    
    // Draw portals
    for (const auto& portal : portals) {
        if (portal.active) {
            renderer->drawPortal(portal.position);
        }
    }
    
    // Draw snake and food
    renderer->drawSnake(snake->getBody());
    renderer->drawFood(food->getPosition());
    
    // Draw score and combo
    renderer->drawScore(score, highScore);
    renderer->drawCombo(snake->getCurrentCombo());
    
    if (hardcoreMode) {
        renderer->drawHardcoreMode();
    }
    
    renderer->refresh();
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
    gameSpeed = std::chrono::milliseconds(200);
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

void Game::checkPortalCollisions() {
    if (snake->isTeleporting()) return;
    
    Point head = snake->getHead();
    for (const auto& portal : portals) {
        if (portal.active && head == portal.position) {
            snake->teleportTo(portal.destination);
            break;
        }
    }
}

void Game::handleFoodEaten() {
    int basePoints = 10;
    int comboMultiplier = snake->getComboMultiplier();
    score += basePoints * comboMultiplier;
    
    if (score > highScore) {
        highScore = score;
        saveHighScore();
    }
    
    snake->grow();
    food->respawn(snake->getBody());
    updateHardcoreSpeed();
}

void Game::toggleHardcoreMode() {
    hardcoreMode = !hardcoreMode;
    if (hardcoreMode) {
        gameSpeed = std::chrono::milliseconds(200);
    }
}

void Game::updateHardcoreSpeed() {
    if (!hardcoreMode) return;
    
    // Increase speed every 3 food items
    if (snake->getLength() % 3 == 0) {
        gameSpeed = std::max(
            std::chrono::milliseconds(50),
            gameSpeed - std::chrono::milliseconds(10)
        );
    }
}

} // namespace SnakeGame 