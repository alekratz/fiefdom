cmake_minimum_required(VERSION 3.6)
project(fiefdom)

# Compiler flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -pedantic -Wall -Wextra -O2")

# Source and executable
set(SOURCE_FILES 
    src/main.cpp src/game.cpp src/game_scene.cpp src/serf.cpp src/game_grid.cpp
    src/buildings.cpp src/toolbar.cpp src/subscenes.cpp src/tools.cpp)
add_executable(fiefdom ${SOURCE_FILES})

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(spdlog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Pre-build command, copy resource directory over
add_custom_command(TARGET fiefdom PRE_BUILD COMMAND cp -r ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:fiefdom>)

# Debug/release mode defines
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG ${CMAKE_CXX_FLAGS}")

# Include dirs and link libraries
include_directories(${SDL2_INCLUDE_DIRS} ${spdlog_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY} pthread)
