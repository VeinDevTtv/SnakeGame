#include "game.h"
#include <windows.h>

int main() {
    // Hide cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    
    // Set console size
    SMALL_RECT windowSize = {0, 0, 80, 40};
    SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);
    
    // Create and run game
    SnakeGame::Game game;
    game.run();
    
    return 0;
} 