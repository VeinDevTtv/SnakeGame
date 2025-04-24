#include "replay.h"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace SnakeGame {

ReplaySystem::ReplaySystem() : recording(false) {}

void ReplaySystem::startRecording(const std::string& playerName) {
    currentReplay = ReplayData();
    currentReplay.playerName = playerName;
    currentReplay.date = std::chrono::system_clock::now();
    currentReplay.finalScore = 0;
    currentReplay.maxCombo = 0;
    recording = true;
    startTime = std::chrono::steady_clock::now();
}

void ReplaySystem::recordMove(Direction dir) {
    if (!recording) return;
    currentReplay.moves.push_back(dir);
}

void ReplaySystem::recordState(const std::deque<Point>& snakeBody, const Point& foodPos, 
                             int score, int combo) {
    if (!recording) return;
    
    GameState state;
    state.snakeBody = snakeBody;
    state.foodPosition = foodPos;
    state.score = score;
    state.combo = combo;
    state.timestamp = std::chrono::steady_clock::now();
    
    currentReplay.states.push_back(state);
    currentReplay.finalScore = score;
    currentReplay.maxCombo = std::max(currentReplay.maxCombo, combo);
}

void ReplaySystem::stopRecording() {
    if (!recording) return;
    
    currentReplay.duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startTime);
    recording = false;
}

bool ReplaySystem::saveReplay(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;
    
    // Write header
    file << "SNAKE_REPLAY_v1\n";
    file << currentReplay.playerName << "\n";
    file << std::chrono::system_clock::to_time_t(currentReplay.date) << "\n";
    file << currentReplay.finalScore << "\n";
    file << currentReplay.maxCombo << "\n";
    file << currentReplay.duration.count() << "\n";
    
    // Write moves
    file << currentReplay.moves.size() << "\n";
    for (const auto& move : currentReplay.moves) {
        file << static_cast<int>(move) << "\n";
    }
    
    // Write states
    file << currentReplay.states.size() << "\n";
    for (const auto& state : currentReplay.states) {
        file << state.snakeBody.size() << "\n";
        for (const auto& point : state.snakeBody) {
            file << point.x << " " << point.y << "\n";
        }
        file << state.foodPosition.x << " " << state.foodPosition.y << "\n";
        file << state.score << "\n";
        file << state.combo << "\n";
        file << std::chrono::steady_clock::to_time_t(state.timestamp) << "\n";
    }
    
    return true;
}

bool ReplaySystem::loadReplay(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;
    
    std::string version;
    std::getline(file, version);
    if (version != "SNAKE_REPLAY_v1") return false;
    
    // Read header
    std::getline(file, currentReplay.playerName);
    
    time_t date;
    file >> date;
    currentReplay.date = std::chrono::system_clock::from_time_t(date);
    
    file >> currentReplay.finalScore;
    file >> currentReplay.maxCombo;
    
    long long duration;
    file >> duration;
    currentReplay.duration = std::chrono::milliseconds(duration);
    
    // Read moves
    size_t moveCount;
    file >> moveCount;
    currentReplay.moves.resize(moveCount);
    for (size_t i = 0; i < moveCount; ++i) {
        int move;
        file >> move;
        currentReplay.moves[i] = static_cast<Direction>(move);
    }
    
    // Read states
    size_t stateCount;
    file >> stateCount;
    currentReplay.states.resize(stateCount);
    for (auto& state : currentReplay.states) {
        size_t bodySize;
        file >> bodySize;
        state.snakeBody.resize(bodySize);
        for (size_t i = 0; i < bodySize; ++i) {
            file >> state.snakeBody[i].x >> state.snakeBody[i].y;
        }
        file >> state.foodPosition.x >> state.foodPosition.y;
        file >> state.score;
        file >> state.combo;
        
        time_t timestamp;
        file >> timestamp;
        state.timestamp = std::chrono::steady_clock::from_time_t(timestamp);
    }
    
    return true;
}

} // namespace SnakeGame 