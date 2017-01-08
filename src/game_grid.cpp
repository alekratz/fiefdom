#include "game_grid.hpp"
#include "globals.hpp"

constexpr auto GRID_STEP = 32;

GameGrid::GameGrid() { }

GameGrid::~GameGrid() { }

void GameGrid::draw() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);

    for(int i = 0; i < GAME_WIDTH + GRID_STEP; i += GRID_STEP) {
        SDL_RenderDrawLine(renderer, (i + x_offset) % (GAME_WIDTH + GRID_STEP), 0, (i + x_offset) % (GAME_WIDTH + GRID_STEP), GAME_HEIGHT);
    }

    for(int i = 0; i < GAME_HEIGHT + GRID_STEP; i += GRID_STEP) {
        SDL_RenderDrawLine(renderer, 0, (i + y_offset) % (GAME_HEIGHT + GRID_STEP), GAME_WIDTH, (i + y_offset) % (GAME_HEIGHT + GRID_STEP));
    }
}

void GameGrid::update() {
}
