#include "game.hpp"
#include "globals.hpp"
#include <SDL.h>
#include <spdlog/spdlog.h>
// #include <SDL_image.h>

namespace spd = spdlog;

SDL_Window* window;
SDL_Renderer* renderer;

bool init_sdl() {
    auto console = spd::stdout_logger_st("init_sdl", true);
    console->debug("Initializing SDL");
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        console->error("Failed to initialize SDL");
        return false;
    }

    return true;
}

bool create_sdl_window() {
    auto console = spd::stdout_logger_st("create_sdl_window", true);
    console->debug("Creating SDL window");
    window = SDL_CreateWindow("Fiefdom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT,
                              SDL_WINDOW_OPENGL);
    if(window == nullptr) {
        console->error("Could not create SDL window");
        return false;
    }
    return true;
}

bool create_sdl_renderer() {
    auto console = spd::stdout_logger_st("create_sdl_renderer", true);
    console->debug("Creating SDL renderer");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return true;
}

int main() {
    // initialize
    spd::set_level(spd::level::debug);
    auto console = spd::stdout_logger_st("main", true);
    if(!(init_sdl() && create_sdl_window() && create_sdl_renderer())) {
        console->critical("Error occurred. Halting.");
        return 1;
    }

    // start game
    Game game;
    game.start();

    // teardown
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}