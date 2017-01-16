#ifndef FIEFDOM_GLOBALS_HPP
#define FIEFDOM_GLOBALS_HPP

#include <SDL.h>
#include <SDL_ttf.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

/*
 * Placement of stuff based on mouse movement
 */
extern int32_t x_offset;    // defined in game_grid.cpp
extern int32_t y_offset;    // defined in game_grid.cpp

extern int32_t player_money;    // defined in game_scene.cpp

#define GAME_WIDTH 1024
#define GAME_HEIGHT 768
#define CURRENCY "shmeckles"

extern TTF_Font* regular_font;
// extern TTF_Font* bold_font;
// extern TTF_Font* italic_font;

#define WHITE_OPAQUE (SDL_Color { 255, 255, 255, 255 })
#define BLACK_OPAQUE (SDL_Color { 0, 0, 0, 255 })

#endif //FIEFDOM_GLOBALS_HPP
