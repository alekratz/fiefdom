cmake_minimum_required(VERSION 3.6)
project(fiefdom)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")

set(SOURCE_FILES src/main.cpp src/game.cpp src/game_scene.cpp src/serf.cpp src/game_grid.cpp src/buildings.cpp)
add_executable(fiefdom ${SOURCE_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(spdlog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${spdlog_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARY} pthread)
