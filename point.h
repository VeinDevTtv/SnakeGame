#pragma once

#include <cmath>

namespace SnakeGame {

class Point {
public:
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    double distanceTo(const Point& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }

    // Wrap coordinates around grid boundaries
    Point wrap(int width, int height) const {
        int newX = (x + width) % width;
        int newY = (y + height) % height;
        return Point(newX, newY);
    }
};

} // namespace SnakeGame 