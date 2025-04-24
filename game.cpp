#include "game.h"
#include <conio.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>
#include <filesystem>

namespace SnakeGame {

Game::Game()
    : score(0), highScore(0), gameOver(false), paused(false),
      gameSpeed(std::chrono::milliseconds(200)), hardcoreMode(false),
      minimalMode(false), portalUseCount(0), currentState(GameState::START_SCREEN) {
    initialize();
}

void Game::run() {
    while (currentState != GameState::EXIT) {
        switch (currentState) {
            case GameState::START_SCREEN:
                showStartScreen();
                break;
            case GameState::CONFIG_SCREEN:
                showConfigScreen();
                break;
            case GameState::PLAYING:
                runGameLoop();
                break;
            case GameState::REPLAY_MENU:
                showReplayMenu();
                break;
            case GameState::ACHIEVEMENTS:
                showAchievements();
                break;
        }
    }
}

void Game::initialize() {
    config = GameConfig::defaultConfig();
    snake = std::make_unique<Snake>(config.width / 2, config.height / 2);
    food = std::make_unique<Food>(config);
    renderer = std::make_unique<Renderer>(config);
    replaySystem = std::make_unique<ReplaySystem>();
    achievementSystem = std::make_unique<AchievementSystem>();
    
    loadHighScore();
    initializePortals();
    
    // Set renderer mode
    renderer->setMinimalMode(minimalMode);
}

void Game::initializePortals() {
    // Create two portals at opposite corners
    portals.clear();
    portals.push_back({Point(1, 1), Point(config.width - 2, config.height - 2), true});
    portals.push_back({Point(config.width - 2, config.height - 2), Point(1, 1), true});
}

void Game::runGameLoop() {
    gameStartTime = std::chrono::steady_clock::now();
    startReplayRecording();
    
    while (!gameOver && currentState == GameState::PLAYING) {
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
    
    if (gameOver) {
        stopReplayRecording();
        updateAchievements();
        renderer->drawGameOver(score);
        _getch(); // Wait for key press
    }
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
                    Direction dir = DirectionManager::fromChar(key);
                    snake->move(dir, config);
                    if (replaySystem->isRecording()) {
                        replaySystem->recordMove(dir);
                    }
                }
                break;
            case 'p':
            case 'P':
                paused = !paused;
                break;
            case 27: // ESC
                if (paused) {
                    currentState = GameState::START_SCREEN;
                } else {
                    paused = true;
                }
                break;
        }
    }
}

void Game::update() {
    if (gameOver || paused) return;
    
    snake->move(snake->getCurrentDirection(), config);
    checkCollisions();
    checkPortalCollisions();
    
    if (snake->getHead() == food->getPosition()) {
        handleFoodEaten();
    }
    
    // Record game state for replay
    if (replaySystem->isRecording()) {
        replaySystem->recordState(snake->getBody(), food->getPosition(), 
                                score, snake->getCurrentCombo());
    }
}

void Game::render() {
    renderer->clear();
    
    if (!minimalMode) {
        // Draw portals
        for (const auto& portal : portals) {
            if (portal.active) {
                renderer->drawPortal(portal.position);
            }
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

void Game::startReplayRecording() {
    replaySystem->startRecording(playerName);
}

void Game::stopReplayRecording() {
    if (replaySystem->isRecording()) {
        replaySystem->stopRecording();
        
        // Save replay if score is high enough
        if (score > 100) {
            std::string filename = "replays/replay_" + 
                std::to_string(std::chrono::system_clock::to_time_t(
                    std::chrono::system_clock::now())) + ".replay";
            replaySystem->saveReplay(filename);
        }
    }
}

void Game::showReplayMenu() {
    renderer->clear();
    renderer->drawString(2, 2, "Replay Menu");
    
    // List available replays
    std::vector<std::string> replays;
    for (const auto& entry : std::filesystem::directory_iterator("replays")) {
        if (entry.path().extension() == ".replay") {
            replays.push_back(entry.path().string());
        }
    }
    
    int selected = 0;
    while (true) {
        renderer->clear();
        renderer->drawString(2, 2, "Select a replay to watch:");
        
        for (size_t i = 0; i < replays.size(); ++i) {
            std::string prefix = (i == selected) ? "> " : "  ";
            renderer->drawString(2, 4 + i, prefix + replays[i]);
        }
        
        char key = _getch();
        switch (key) {
            case 'w':
            case 'W':
                selected = (selected > 0) ? selected - 1 : replays.size() - 1;
                break;
            case 's':
            case 'S':
                selected = (selected + 1) % replays.size();
                break;
            case 13: // Enter
                if (!replays.empty()) {
                    playReplay(replays[selected]);
                }
                break;
            case 27: // ESC
                currentState = GameState::START_SCREEN;
                return;
        }
    }
}

void Game::playReplay(const std::string& filename) {
    if (!replaySystem->loadReplay(filename)) {
        renderer->drawString(2, 2, "Failed to load replay!");
        _getch();
        return;
    }
    
    const auto& replay = replaySystem->getCurrentReplay();
    size_t currentState = 0;
    
    while (currentState < replay.states.size()) {
        const auto& state = replay.states[currentState];
        
        renderer->clear();
        renderer->drawSnake(state.snakeBody);
        renderer->drawFood(state.foodPosition);
        renderer->drawScore(state.score, highScore);
        renderer->drawCombo(state.combo);
        renderer->refresh();
        
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) break; // ESC
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        currentState++;
    }
}

void Game::updateAchievements() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - gameStartTime);
    
    achievementSystem->update(score, snake->getCurrentCombo(), 
                            snake->getLength(), duration);
}

void Game::showAchievements() {
    renderer->clear();
    renderer->drawString(2, 2, "Achievements");
    
    const auto& achievements = achievementSystem->getAchievements();
    for (size_t i = 0; i < achievements.size(); ++i) {
        const auto& achievement = achievements[i];
        std::string status = achievement.unlocked ? "[X]" : "[ ]";
        renderer->drawString(2, 4 + i, status + " " + achievement.name);
        renderer->drawString(6, 4 + i, achievement.description);
    }
    
    renderer->drawString(2, 4 + achievements.size() + 1, 
                        "Press any key to return to menu");
    _getch();
    currentState = GameState::START_SCREEN;
}

void Game::toggleMinimalMode() {
    minimalMode = !minimalMode;
    renderer->setMinimalMode(minimalMode);
}

} // namespace SnakeGame 