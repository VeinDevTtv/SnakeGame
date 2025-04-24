#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <functional>

namespace SnakeGame {

struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    bool unlocked;
    std::chrono::system_clock::time_point unlockDate;
    
    // Achievement conditions
    std::function<bool(int score, int combo, int length, 
                      std::chrono::milliseconds duration)> checkCondition;
};

class AchievementSystem {
public:
    AchievementSystem();
    
    void initialize();
    void update(int score, int combo, int snakeLength, 
               std::chrono::milliseconds duration);
    void saveAchievements() const;
    void loadAchievements();
    
    const std::vector<Achievement>& getAchievements() const { return achievements; }
    bool isAchievementUnlocked(const std::string& id) const;
    
private:
    std::vector<Achievement> achievements;
    
    void createAchievements();
    void checkAchievements(int score, int combo, int snakeLength, 
                          std::chrono::milliseconds duration);
};

} // namespace SnakeGame 