#include "food.h"
#include <algorithm>

namespace SnakeGame {

Food::Food(const GameConfig& config)
    : type(FoodType::NORMAL) {
    std::random_device rd;
    rng.seed(rd());
}

void Food::place(const std::vector<Point>& snakeBody, const GameConfig& config) {
    position = generatePosition(config.width, config.height, snakeBody);
    type = generateFoodType(config);
}

char Food::getDisplayChar() const {
    switch (type) {
        case FoodType::NORMAL: return FOOD;
        case FoodType::SPEED_BOOST: return SPEED_FOOD;
        case FoodType::REVERSE_CONTROLS: return REVERSE_FOOD;
        default: return FOOD;
    }
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

FoodType Food::generateFoodType(const GameConfig& config) {
    if (!config.enableSpecialFood) return FoodType::NORMAL;
    
    std::uniform_int_distribution<> dis(0, 100);
    int chance = dis(rng);
    
    if (chance < 80) return FoodType::NORMAL;
    if (chance < 90) return FoodType::SPEED_BOOST;
    return FoodType::REVERSE_CONTROLS;
}

Point Food::generatePosition(int width, int height, const std::vector<Point>& snakeBody) {
    std::uniform_int_distribution<> disX(1, width - 2);
    std::uniform_int_distribution<> disY(1, height - 2);
    
    Point newPos;
    do {
        newPos = Point(disX(rng), disY(rng));
    } while (std::find(snakeBody.begin(), snakeBody.end(), newPos) != snakeBody.end());
    
    return newPos;
}

} // namespace SnakeGame 