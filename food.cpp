#include "food.h"
#include <random>
#include <algorithm>

namespace SnakeGame {

Food::Food(const GameConfig& config) 
    : type(FoodType::NORMAL)
    , displayChar(FOOD)
    , config(config) {
    std::random_device rd;
    rng.seed(rd());
}

void Food::place(const std::deque<Point>& snakeBody, const GameConfig& config) {
    this->config = config;
    type = generateFoodType(config);
    position = generatePosition(config.width, config.height, snakeBody);
    updateDisplayChar();
}

void Food::respawn(const std::deque<Point>& snakeBody) {
    type = generateFoodType(config);
    position = generatePosition(config.width, config.height, snakeBody);
    updateDisplayChar();
}

FoodType Food::generateFoodType(const GameConfig& config) {
    if (!config.enableSpecialFood) return FoodType::NORMAL;
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double chance = dist(rng);
    
    if (chance < 0.1) return FoodType::SPEED_BOOST;
    if (chance < 0.2) return FoodType::REVERSE_CONTROLS;
    return FoodType::NORMAL;
}

Point Food::generatePosition(int width, int height, const std::deque<Point>& snakeBody) {
    std::uniform_int_distribution<int> xDist(1, width - 2);
    std::uniform_int_distribution<int> yDist(1, height - 2);
    
    Point newPos;
    do {
        newPos = Point(xDist(rng), yDist(rng));
    } while (!isValidPosition(newPos, snakeBody));
    
    return newPos;
}

bool Food::isValidPosition(const Point& pos, const std::deque<Point>& snakeBody) const {
    return std::find(snakeBody.begin(), snakeBody.end(), pos) == snakeBody.end();
}

void Food::updateDisplayChar() {
    switch (type) {
        case FoodType::NORMAL:
            displayChar = FOOD;
            break;
        case FoodType::SPEED_BOOST:
            displayChar = SPEED_FOOD;
            break;
        case FoodType::REVERSE_CONTROLS:
            displayChar = REVERSE_FOOD;
            break;
    }
}

char Food::getDisplayChar() const {
    return displayChar;
}

std::chrono::milliseconds Food::getEffectDuration() const {
    switch (type) {
        case FoodType::SPEED_BOOST:
        case FoodType::REVERSE_CONTROLS:
            return std::chrono::seconds(5);
        default:
            return std::chrono::milliseconds(0);
    }
}

} // namespace SnakeGame 