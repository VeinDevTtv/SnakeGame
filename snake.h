#pragma once

#include <vector>
#include <memory>
#include "point.h"
#include "direction.h"

namespace SnakeGame {

class Snake {
public:
    Snake(int startX, int startY, int initialLength = 3);
    
    void move(Direction dir, const GameConfig& config);
    void grow();
    bool checkCollision(const Point& point) const;
    bool checkSelfCollision() const;
    bool checkWallCollision(const GameConfig& config) const;
    
    const std::vector<Point>& getBody() const { return body; }
    Point getHead() const { return body.front(); }
    int getLength() const { return body.size(); }
    Direction getCurrentDirection() const { return currentDirection; }

private:
    std::vector<Point> body;
    Direction currentDirection;
    bool isReversed;

    void updatePosition(const Point& newHead, const GameConfig& config);
};

} // namespace SnakeGame 