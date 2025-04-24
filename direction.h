#pragma once

#include "point.h"

namespace SnakeGame {

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class DirectionManager {
public:
    static Point getDirectionVector(Direction dir) {
        switch (dir) {
            case Direction::UP:    return Point(0, -1);
            case Direction::DOWN:  return Point(0, 1);
            case Direction::LEFT:  return Point(-1, 0);
            case Direction::RIGHT: return Point(1, 0);
            default:              return Point(0, 0);
        }
    }

    static bool isOpposite(Direction dir1, Direction dir2) {
        return (dir1 == Direction::UP && dir2 == Direction::DOWN) ||
               (dir1 == Direction::DOWN && dir2 == Direction::UP) ||
               (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) ||
               (dir1 == Direction::RIGHT && dir2 == Direction::LEFT);
    }

    static Direction fromChar(char c) {
        switch (c) {
            case 'w':
            case 'W': return Direction::UP;
            case 's':
            case 'S': return Direction::DOWN;
            case 'a':
            case 'A': return Direction::LEFT;
            case 'd':
            case 'D': return Direction::RIGHT;
            default:  return Direction::NONE;
        }
    }
};

} // namespace SnakeGame 