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
    constexpr auto GRID_MOVE_SPEED = 0.05;
    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(ev.type == SDL_MOUSEMOTION) {
            x_offset = (ev.motion.x - (GAME_WIDTH / 2)) * GRID_MOVE_SPEED;
            y_offset = (ev.motion.y - (GAME_HEIGHT / 2)) * GRID_MOVE_SPEED;
        }
    }
}
