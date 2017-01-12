#ifndef FIEFDOM_GLOBALS_HPP
#define FIEFDOM_GLOBALS_HPP

#include <SDL.h>
#include <SDL_ttf.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern int32_t x_offset;
extern int32_t y_offset;

#define GAME_WIDTH 1024
#define GAME_HEIGHT 768

extern TTF_Font* regular_font;
// extern TTF_Font* bold_font;
// extern TTF_Font* italic_font;

#define WHITE_OPAQUE (SDL_Color { 255, 255, 255, 255 })
#define BLACK_OPAQUE (SDL_Color { 0, 0, 0, 255 })

#endif //FIEFDOM_GLOBALS_HPP
