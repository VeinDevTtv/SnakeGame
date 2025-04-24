#include "achievements.h"
#include <fstream>
#include <json/json.h>

namespace SnakeGame {

AchievementSystem::AchievementSystem() {
    initialize();
}

void AchievementSystem::initialize() {
    createAchievements();
    loadAchievements();
}

void AchievementSystem::createAchievements() {
    achievements = {
        {
            "first_game",
            "First Steps",
            "Complete your first game",
            false,
            std::chrono::system_clock::time_point(),
            [](int score, int combo, int length, std::chrono::milliseconds duration) {
                return score > 0;
            }
        },
        {
            "speed_demon",
            "Speed Demon",
            "Survive for 100 seconds in hardcore mode",
            false,
            std::chrono::system_clock::time_point(),
            [](int score, int combo, int length, std::chrono::milliseconds duration) {
                return duration >= std::chrono::seconds(100);
            }
        },
        {
            "combo_master",
            "Combo Master",
            "Achieve a 5x combo",
            false,
            std::chrono::system_clock::time_point(),
            [](int score, int combo, int length, std::chrono::milliseconds duration) {
                return combo >= 5;
            }
        },
        {
            "snake_king",
            "Snake King",
            "Reach a length of 20",
            false,
            std::chrono::system_clock::time_point(),
            [](int score, int combo, int length, std::chrono::milliseconds duration) {
                return length >= 20;
            }
        },
        {
            "portal_master",
            "Portal Master",
            "Use portals 10 times in a single game",
            false,
            std::chrono::system_clock::time_point(),
            [](int score, int combo, int length, std::chrono::milliseconds duration) {
                // This will be updated by the game when portals are used
                return false;
            }
        },
        {
            "score_hunter",
            "Score Hunter",
            "Reach a score of 1000",
            false,
            std::chrono::system_clock::time_point(),
            [](int score, int combo, int length, std::chrono::milliseconds duration) {
                return score >= 1000;
            }
        }
    };
}

void AchievementSystem::update(int score, int combo, int snakeLength, 
                             std::chrono::milliseconds duration) {
    checkAchievements(score, combo, snakeLength, duration);
}

void AchievementSystem::checkAchievements(int score, int combo, int snakeLength, 
                                        std::chrono::milliseconds duration) {
    for (auto& achievement : achievements) {
        if (!achievement.unlocked && 
            achievement.checkCondition(score, combo, snakeLength, duration)) {
            achievement.unlocked = true;
            achievement.unlockDate = std::chrono::system_clock::now();
            saveAchievements();
        }
    }
}

void AchievementSystem::saveAchievements() const {
    Json::Value root;
    for (const auto& achievement : achievements) {
        Json::Value achievementJson;
        achievementJson["id"] = achievement.id;
        achievementJson["unlocked"] = achievement.unlocked;
        if (achievement.unlocked) {
            achievementJson["unlockDate"] = std::chrono::system_clock::to_time_t(achievement.unlockDate);
        }
        root.append(achievementJson);
    }
    
    std::ofstream file("achievements.json");
    if (file.is_open()) {
        Json::StyledWriter writer;
        file << writer.write(root);
    }
}

void AchievementSystem::loadAchievements() {
    std::ifstream file("achievements.json");
    if (!file.is_open()) return;
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(file, root)) return;
    
    for (const auto& achievementJson : root) {
        std::string id = achievementJson["id"].asString();
        for (auto& achievement : achievements) {
            if (achievement.id == id) {
                achievement.unlocked = achievementJson["unlocked"].asBool();
                if (achievement.unlocked) {
                    time_t date = achievementJson["unlockDate"].asInt64();
                    achievement.unlockDate = std::chrono::system_clock::from_time_t(date);
                }
                break;
            }
        }
    }
}

bool AchievementSystem::isAchievementUnlocked(const std::string& id) const {
    for (const auto& achievement : achievements) {
        if (achievement.id == id) {
            return achievement.unlocked;
        }
    }
    return false;
}

} // namespace SnakeGame 