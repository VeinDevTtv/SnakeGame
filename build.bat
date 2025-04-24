@echo off
echo Building Snake Game...

:: Create build directory if it doesn't exist
if not exist build mkdir build
cd build

:: Run CMake
cmake ..

:: Build the project
cmake --build .

:: Create necessary directories
if not exist replays mkdir replays

:: Copy configuration files
copy ..\highscore.txt .
copy ..\achievements.json .

echo Build complete!
echo Run the game with: build\Debug\snake_game.exe
pause 