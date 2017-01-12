#include "subscenes.hpp"
#include "globals.hpp"
#include "util.hpp"

#include <iostream>
using namespace std;

const auto DRAW_BOX_WIDTH = 360;
const auto DRAW_BOX_HEIGHT = 240;

YesNoSubscene::YesNoSubscene(cstref prompt)
    : Entity()
    , m_draw_box({CENTER(GAME_WIDTH, DRAW_BOX_WIDTH), CENTER(GAME_HEIGHT, DRAW_BOX_HEIGHT), 
            DRAW_BOX_WIDTH, DRAW_BOX_HEIGHT})
    , m_done(false) {
        m_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STATIC | SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);
        SDL_SetRenderTarget(renderer, m_box_texture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_Rect outline {1, 1, GAME_WIDTH - 2, GAME_HEIGHT - 2};
        SDL_RenderDrawRect(renderer, &outline);

        auto prompt_surface = TTF_RenderText_Shaded(regular_font, prompt.c_str(), BLACK_OPAQUE, WHITE_OPAQUE);
        assert(prompt_surface);
        m_prompt_texture = SDL_CreateTextureFromSurface(renderer, prompt_surface);
        assert(m_prompt_texture);

        m_text_center.w = prompt_surface->w;
        m_text_center.h = prompt_surface->h;
        m_text_center.x = CENTER(GAME_WIDTH, m_text_center.w);
        m_text_center.y = CENTER(GAME_HEIGHT, m_text_center.h);

        SDL_FreeSurface(prompt_surface);
        SDL_SetRenderTarget(renderer, nullptr);
    }

YesNoSubscene::~YesNoSubscene() {
    SDL_DestroyTexture(m_box_texture);
    SDL_DestroyTexture(m_prompt_texture);
}

void YesNoSubscene::draw() { 
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, m_box_texture, nullptr, &m_draw_box);
    SDL_RenderCopy(renderer, m_prompt_texture, nullptr, &m_text_center);
}

void YesNoSubscene::update() { 
    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_y) {
            m_result = true;
            m_done = true;
        }
        else if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_n) {
            m_result = false;
            m_done = true;
        }
    }
}
