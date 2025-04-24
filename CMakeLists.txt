cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JsonCpp package
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Add source files
set(SOURCES
    main.cpp
    game.cpp
    snake.cpp
    food.cpp
    renderer.cpp
    replay.cpp
    achievements.cpp
)

# Add header files
set(HEADERS
    game.h
    snake.h
    food.h
    renderer.h
    replay.h
    achievements.h
    point.h
    direction.h
    constants.h
)

# Create executable
add_executable(snake_game ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(snake_game ${JSONCPP_LIBRARIES})

# Include directories
target_include_directories(snake_game PRIVATE ${JSONCPP_INCLUDE_DIRS})

# Create replays directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/replays)

# Copy configuration files to build directory
configure_file(${CMAKE_SOURCE_DIR}/highscore.txt ${CMAKE_BINARY_DIR}/highscore.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/achievements.json ${CMAKE_BINARY_DIR}/achievements.json COPYONLY) 