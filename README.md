# Snake Game

A modern implementation of the classic Snake game with advanced features and visual effects.

## Features

### Core Gameplay
- Classic snake movement and growth mechanics
- Food collection and score tracking
- Wall collision detection
- Wrap-around mode option
- Hardcore mode with increasing speed

### Visual Effects
- ASCII art title screen and game over screen
- Animated score count-up
- Combo system with visual feedback
- Portal visualization
- Minimal mode option for reduced visual effects

### Advanced Features
- Combo system: Chain food collection for bonus points
- Portal system: Teleport between opposite corners
- Hardcore mode: Increasing speed challenge
- Achievement system with unlockable goals
- Replay system to save and watch past games
- High score tracking with player names and dates

### Technical Features
- Performance optimized using std::deque for snake body
- Configurable game settings
- Cross-platform input handling
- Save/load system for high scores and achievements
- Replay system for game analysis

## Controls

### In-Game Controls
- W/↑: Move Up
- S/↓: Move Down
- A/←: Move Left
- D/→: Move Right
- P: Pause/Resume
- ESC: Quit Game

### Menu Controls
- Arrow Keys: Navigate menus
- ENTER: Select option
- ESC: Back/Exit

### Configuration Options
- Grid Size: 20x20 or 30x30
- Game Mode: Classic or Wrap-around
- Difficulty: Easy, Normal, Hard
- Special Food: On/Off
- Animations: On/Off
- Minimal Mode: On/Off

## Achievements

- First Steps: Complete your first game
- Speed Demon: Survive for 100 seconds in hardcore mode
- Combo Master: Achieve a 5x combo
- Snake King: Reach a length of 20
- Portal Master: Use portals 10 times in a single game
- Score Hunter: Reach a score of 1000

## Replay System

The game includes a replay system that allows you to:
- Save your best games
- Watch replays with playback controls
- Analyze your gameplay
- Share replays with friends

## Building and Running

### Requirements
- C++17 or later
- Windows (for console-specific features)
- JsonCpp library (for achievements)

### Building
```bash
mkdir build
cd build
cmake ..
make
```

### Running
```bash
./snake_game
```

## Configuration Files

- `highscore.txt`: Stores high scores with player names and dates
- `achievements.json`: Tracks unlocked achievements
- `replays/`: Directory containing saved game replays

## Contributing

Feel free to submit issues and enhancement requests! 