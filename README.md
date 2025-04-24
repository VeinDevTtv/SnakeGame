# Terminal Snake Game

A classic Snake game that runs in the terminal, written in C++.

## Features

- Classic snake gameplay with walls
- Score tracking and high score saving
- Pause functionality
- Gradually increasing difficulty
- Smooth controls using WASD keys

## Requirements

- Windows OS (uses Windows-specific headers)
- C++ compiler (e.g., MinGW, Visual Studio)

## How to Compile

Using MinGW:
```bash
g++ snake_game.cpp -o snake_game
```

Using Visual Studio:
1. Create a new C++ Console Application
2. Replace the contents of the main .cpp file with the game code
3. Build the solution

## How to Play

1. Run the compiled executable
2. Use WASD keys to control the snake:
   - W: Move up
   - A: Move left
   - S: Move down
   - D: Move right
3. Press P to pause/resume the game
4. Press ESC to quit

## Game Rules

- The snake grows when it eats food (@)
- The game ends if the snake hits the wall or itself
- Score increases by 1 for each food eaten
- Speed increases every 5 points
- High scores are saved to a file

## Controls

- WASD: Move the snake
- P: Pause/Resume game
- ESC: Quit game

## Notes

- The game runs in a 20x20 grid
- The snake is represented by 'O' (head) and 'o' (body)
- Food is represented by '@'
- Walls are represented by '#'
- Empty spaces are represented by '.' 