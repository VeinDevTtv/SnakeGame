#pragma once

#include <deque>
#include <memory>
#include <chrono>
#include "point.h"
#include "direction.h"

namespace SnakeGame {

struct ComboState {
    int currentCombo;
    std::chrono::steady_clock::time_point lastFoodTime;
    static constexpr int COMBO_WINDOW_MS = 2000; // 2 second window for combos
};

class Snake {
public:
    Snake(int startX, int startY, int initialLength = 3);
    
    void move(Direction dir, const GameConfig& config);
    void grow();
    bool checkCollision(const Point& point) const;
    bool checkSelfCollision() const;
    bool checkWallCollision(const GameConfig& config) const;
    
    const std::deque<Point>& getBody() const { return body; }
    Point getHead() const { return body.front(); }
    int getLength() const { return body.size(); }
    Direction getCurrentDirection() const { return currentDirection; }
    
    // Combo system
    int getCurrentCombo() const { return comboState.currentCombo; }
    void updateCombo();
    int getComboMultiplier() const;
    
    // Portal system
    void teleportTo(const Point& newPosition);
    bool isTeleporting() const { return isInPortal; }
    void setTeleporting(bool value) { isInPortal = value; }

private:
    std::deque<Point> body;
    Direction currentDirection;
    bool isReversed;
    bool isInPortal;
    ComboState comboState;

    void updatePosition(const Point& newHead, const GameConfig& config);
};

} // namespace SnakeGame 