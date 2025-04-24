#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <deque>
#include "point.h"
#include "direction.h"

namespace SnakeGame {

struct GameState {
    std::deque<Point> snakeBody;
    Point foodPosition;
    int score;
    int combo;
    std::chrono::steady_clock::time_point timestamp;
};

struct ReplayData {
    std::string playerName;
    std::chrono::system_clock::time_point date;
    std::vector<GameState> states;
    std::vector<Direction> moves;
    int finalScore;
    int maxCombo;
    std::chrono::milliseconds duration;
};

class ReplaySystem {
public:
    ReplaySystem();
    
    void startRecording(const std::string& playerName);
    void recordMove(Direction dir);
    void recordState(const std::deque<Point>& snakeBody, const Point& foodPos, 
                    int score, int combo);
    void stopRecording();
    
    bool saveReplay(const std::string& filename) const;
    bool loadReplay(const std::string& filename);
    
    const ReplayData& getCurrentReplay() const { return currentReplay; }
    bool isRecording() const { return recording; }
    
private:
    ReplayData currentReplay;
    bool recording;
    std::chrono::steady_clock::time_point startTime;
};

} // namespace SnakeGame 