#include "buildings.hpp"
#include "globals.hpp"

Farm::Farm(const SDL_Rect& dims)
    : dims(dims)
    , yield_per_area(0.1) { }

void Farm::draw() {
    SDL_Rect draw_rect = dims;
    draw_rect.x += x_offset;
    draw_rect.y += y_offset;
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 212, 168, 144, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &draw_rect);
    SDL_SetRenderDrawColor(renderer, 154, 212, 144, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &draw_rect);
}

void Farm::update() {
    
}