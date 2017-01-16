#include "buildings.hpp"
#include "globals.hpp"

const SDL_Color TEXT_FG = BLACK_OPAQUE;
const SDL_Color TEXT_BG { 128, 128, 128, 128 };

Farm::Farm(const SDL_Rect& dims)
    : dims(dims)
    , yield_per_area(0.1)
    , m_text_texture(nullptr) {
        /* TODO : make these static and load on first use */
    SDL_Surface* text_surface = TTF_RenderText_Shaded(regular_font, "FARM", TEXT_FG, TEXT_BG);
    m_text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
}

Farm::~Farm() {
    SDL_DestroyTexture(m_text_texture);
}

void Farm::draw() {
    SDL_Rect draw_rect = dims;
    draw_rect.x += x_offset;
    draw_rect.y += y_offset;
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 212, 168, 144, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &draw_rect);
    SDL_SetRenderDrawColor(renderer, 154, 212, 144, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &draw_rect);
    SDL_QueryTexture(m_text_texture, nullptr, nullptr, &draw_rect.w, &draw_rect.h);
    // normally I'd center this, but it's just temporary labeling and akin to programmer art.
    SDL_RenderCopy(renderer, m_text_texture, nullptr, &draw_rect);
}

void Farm::update() {
    
}
