#include "subscenes.hpp"
#include "globals.hpp"
#include "util.hpp"

const auto DRAW_BOX_WIDTH = 360;
const auto DRAW_BOX_HEIGHT = 240;

YesNoSubscene::YesNoSubscene()
    : Subscene()
    , m_draw_box({CENTER(GAME_WIDTH, DRAW_BOX_WIDTH), CENTER(GAME_HEIGHT, DRAW_BOX_HEIGHT), 
            DRAW_BOX_WIDTH, DRAW_BOX_HEIGHT}) {
        m_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STATIC | SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);
        SDL_SetRenderTarget(renderer, m_box_texture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &m_draw_box);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(renderer, &m_draw_box);
    }

YesNoSubscene::~YesNoSubscene() {
    SDL_DestroyTexture(m_box_texture);
}

void YesNoSubscene::draw() { 
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, m_box_texture, nullptr, &m_draw_box);
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
