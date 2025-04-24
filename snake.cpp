#include "snake.h"
#include "constants.h"
#include <algorithm>

namespace SnakeGame {

Snake::Snake(int startX, int startY, int initialLength)
    : currentDirection(Direction::RIGHT)
    , isReversed(false)
    , isInPortal(false)
    , comboState{0, std::chrono::steady_clock::now()} {
    // Initialize snake body
    for (int i = 0; i < initialLength; ++i) {
        body.push_front(Point(startX - i, startY));
    }
}

void Snake::move(Direction dir, const GameConfig& config) {
    if (isInPortal) return; // Don't move while teleporting
    
    Point newHead = getHead();
    
    // Update direction
    if (!isReversed) {
        currentDirection = dir;
    } else {
        // Reverse the direction if snake is reversed
        switch (dir) {
            case Direction::UP: currentDirection = Direction::DOWN; break;
            case Direction::DOWN: currentDirection = Direction::UP; break;
            case Direction::LEFT: currentDirection = Direction::RIGHT; break;
            case Direction::RIGHT: currentDirection = Direction::LEFT; break;
        }
    }
    
    // Calculate new head position
    switch (currentDirection) {
        case Direction::UP: newHead.y--; break;
        case Direction::DOWN: newHead.y++; break;
        case Direction::LEFT: newHead.x--; break;
        case Direction::RIGHT: newHead.x++; break;
    }
    
    updatePosition(newHead, config);
}

void Snake::grow() {
    // Add new segment at the end
    body.push_back(body.back());
    
    // Update combo
    updateCombo();
}

void Snake::updateCombo() {
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastFood = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - comboState.lastFoodTime).count();
    
    if (timeSinceLastFood < ComboState::COMBO_WINDOW_MS) {
        comboState.currentCombo++;
    } else {
        comboState.currentCombo = 1;
    }
    
    comboState.lastFoodTime = now;
}

int Snake::getComboMultiplier() const {
    return std::min(comboState.currentCombo, 5); // Cap at 5x multiplier
}

void Snake::teleportTo(const Point& newPosition) {
    isInPortal = true;
    body.front() = newPosition;
}

void Snake::updatePosition(const Point& newHead, const GameConfig& config) {
    // Handle wrapping around the screen
    if (config.wrapAround) {
        if (newHead.x < 0) newHead.x = config.width - 1;
        if (newHead.x >= config.width) newHead.x = 0;
        if (newHead.y < 0) newHead.y = config.height - 1;
        if (newHead.y >= config.height) newHead.y = 0;
    }
    
    body.push_front(newHead);
    body.pop_back();
}

bool Snake::checkCollision(const Point& point) const {
    return std::find(body.begin(), body.end(), point) != body.end();
}

bool Snake::checkSelfCollision() const {
    auto head = body.front();
    return std::find(body.begin() + 1, body.end(), head) != body.end();
}

bool Snake::checkWallCollision(const GameConfig& config) const {
    if (config.wrapAround) return false;
    
    auto head = body.front();
    return head.x < 0 || head.x >= config.width || 
           head.y < 0 || head.y >= config.height;
}

} // namespace SnakeGame 