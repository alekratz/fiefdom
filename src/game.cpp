#include "game.hpp"
#include "game_grid.hpp"
#include "globals.hpp"
#include <SDL.h>

constexpr auto TARGET_FPS = 60;
constexpr auto MS_PER_FRAME = 1000 / TARGET_FPS;

int32_t x_offset = 0;
int32_t y_offset = 0;

Game::Game()
    : Loggable("game")
    , m_is_running(false)
    , m_game_scene(std::make_unique<GameScene>(m_is_running)) {
    
}

void Game::start() {
    m_logger->debug("Starting game");
    assert(!m_is_running);
    
    uint32_t start, end, delta;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    m_is_running = true;
    while(m_is_running) {
        start = SDL_GetTicks();
        update();
        draw();
        end = SDL_GetTicks();
        delta = end - start;
        //m_logger->debug("{} ms", delta);
        if(delta < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME - delta);
        }
    }
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // draw stuff here
    m_game_scene->draw();
    SDL_RenderPresent(renderer);
}

void Game::update() {
    SDL_PumpEvents();
    m_game_scene->update();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}
