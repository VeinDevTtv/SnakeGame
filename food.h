#pragma once

#include "point.h"
#include "constants.h"
#include <random>
#include <chrono>

namespace SnakeGame {

class Food {
public:
    Food(const GameConfig& config);
    
    void place(const std::vector<Point>& snakeBody, const GameConfig& config);
    const Point& getPosition() const { return position; }
    FoodType getType() const { return type; }
    char getDisplayChar() const;
    
    bool isSpecial() const { return type != FoodType::NORMAL; }
    std::chrono::milliseconds getEffectDuration() const;

private:
    Point position;
    FoodType type;
    std::mt19937 rng;
    
    FoodType generateFoodType(const GameConfig& config);
    Point generatePosition(int width, int height, const std::vector<Point>& snakeBody);
};

} // namespace SnakeGame 