#include "game.hpp"
#include "globals.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <spdlog/spdlog.h>
// #include <SDL_image.h>

namespace spd = spdlog;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* regular_font;

bool init_sdl() {
    auto console = spd::stdout_color_mt("init_sdl");
    console->debug("Initializing SDL");
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        console->error("Failed to initialize SDL");
        return false;
    }

    console->debug("Initializing SDL_ttf");
    if(TTF_Init() == -1) {
        console->error("Failed to initialize SDL_ttf");
        return false;
    }

    return true;
}

bool create_sdl_window() {
    auto console = spd::stdout_color_mt("create_sdl_window");
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
    auto console = spd::stdout_color_mt("create_sdl_renderer");
    console->debug("Creating SDL renderer");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return true;
}

bool load_base_assets() {
    auto console = spd::stdout_color_mt("load_base_assets");
    console->debug("Loading base assets");
    constexpr auto REGULAR_FONT_PATH = "res/font/OpenSans-Regular.ttf";

    console->debug("Loading {}", REGULAR_FONT_PATH);
    regular_font = TTF_OpenFont(REGULAR_FONT_PATH, 12);
    if(regular_font == nullptr) {
        console->error("Could not load {}: {}", REGULAR_FONT_PATH, TTF_GetError());
        return false;
    }
    return true;
}

int main() {
    // initialize
    spd::set_level(spd::level::debug);
    auto console = spd::stdout_color_mt("main");
    if(!(init_sdl() && load_base_assets() && create_sdl_window() && create_sdl_renderer())) {
        console->critical("Error occurred. Halting.");
        return 1;
    }

    // start game
    Game game;
    game.start();

    // teardown
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
