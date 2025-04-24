#include "snake.h"
#include "constants.h"
#include <algorithm>

namespace SnakeGame {

Snake::Snake(int startX, int startY, int initialLength)
    : currentDirection(Direction::RIGHT), isReversed(false) {
    // Initialize snake body
    for (int i = 0; i < initialLength; ++i) {
        body.push_back(Point(startX - i, startY));
    }
}

void Snake::move(Direction dir, const GameConfig& config) {
    if (dir == Direction::NONE) return;
    
    // Don't allow 180-degree turns
    if (DirectionManager::isOpposite(dir, currentDirection)) return;
    
    currentDirection = dir;
    Point directionVector = DirectionManager::getDirectionVector(dir);
    if (isReversed) {
        directionVector = Point(-directionVector.x, -directionVector.y);
    }
    
    Point newHead = body.front() + directionVector;
    
    // Handle wrap-around mode
    if (config.mode == GameMode::WRAP_AROUND) {
        newHead = newHead.wrap(config.width, config.height);
    }
    
    updatePosition(newHead, config);
}

void Snake::grow() {
    // Add new segment at the tail position
    body.push_back(body.back());
}

bool Snake::checkCollision(const Point& point) const {
    return std::find(body.begin(), body.end(), point) != body.end();
}

bool Snake::checkSelfCollision() const {
    const Point& head = body.front();
    return std::find(body.begin() + 1, body.end(), head) != body.end();
}

bool Snake::checkWallCollision(const GameConfig& config) const {
    if (config.mode == GameMode::WRAP_AROUND) return false;
    
    const Point& head = body.front();
    return head.x <= 0 || head.x >= config.width - 1 ||
           head.y <= 0 || head.y >= config.height - 1;
}

void Snake::updatePosition(const Point& newHead, const GameConfig& config) {
    // Move body
    for (size_t i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    body[0] = newHead;
}

} // namespace SnakeGame 