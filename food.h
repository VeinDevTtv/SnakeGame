#pragma once

#include <deque>
#include "point.h"
#include "constants.h"
#include <random>
#include <chrono>

namespace SnakeGame {

class Food {
public:
    Food(const GameConfig& config);
    
    void place(const std::deque<Point>& snakeBody, const GameConfig& config);
    void respawn(const std::deque<Point>& snakeBody);
    const Point& getPosition() const { return position; }
    FoodType getType() const { return type; }
    char getDisplayChar() const { return displayChar; }
    
    bool isSpecial() const { return type != FoodType::NORMAL; }
    std::chrono::milliseconds getEffectDuration() const;

private:
    Point position;
    FoodType type;
    char displayChar;
    GameConfig config;
    std::mt19937 rng;
    
    FoodType generateFoodType(const GameConfig& config);
    Point generatePosition(int width, int height, const std::deque<Point>& snakeBody);
    bool isValidPosition(const Point& pos, const std::deque<Point>& snakeBody) const;
    void updateDisplayChar();
};

} // namespace SnakeGame 