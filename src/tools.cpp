#include "tools.hpp"
#include "globals.hpp"
#include "game_grid.hpp"
#include <cassert>
#include <SDL.h>

constexpr auto POINT_W = 10;
constexpr auto POINT_H = 10;

DrawTool::DrawTool()
    : x_start(-POINT_W)
    , y_start(-POINT_H)
    , x_end(-POINT_W)
    , y_end(-POINT_H)
    , started(false) 
    , ended(false)
    , m_done(false)
    , m_point(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET | SDL_TEXTUREACCESS_STATIC,
                POINT_W, POINT_H)) {
    assert(m_point);
    SDL_SetRenderTarget(renderer, m_point);
    SDL_SetRenderDrawColor(renderer, 100, 128, 255, 128);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}

DrawTool::~DrawTool() {

}

void DrawTool::reset() {
    started = ended = m_done = false;
    x_start = x_end = -POINT_W;
    y_start = y_end = -POINT_H;
}

void DrawTool::draw() {
    //draw_rect.x = grid_snap
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_Rect start_rect { x_start + x_offset, y_start + y_offset, POINT_W, POINT_H };
    SDL_RenderCopy(renderer, m_point, nullptr, &start_rect);

    if (started) {
        // clicked once; waiting to create a new side
        SDL_Rect end_rect { x_end + x_offset, y_end + y_offset, POINT_W, POINT_H };
        SDL_Rect aux_rect1 { x_start + x_offset, y_end + y_offset, POINT_W, POINT_H };
        SDL_Rect aux_rect2 { x_end + x_offset, y_start + y_offset, POINT_W, POINT_H };
        SDL_RenderCopy(renderer, m_point, nullptr, &end_rect);
        SDL_RenderCopy(renderer, m_point, nullptr, &aux_rect1);
        SDL_RenderCopy(renderer, m_point, nullptr, &aux_rect2);
    }
}

void DrawTool::update() {
    int32_t mouse_x, mouse_y;
    mouse_x = mouse_y = -1;
    bool mouse_down = false;
    bool mouse_motion = false;

    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(ev.type == SDL_MOUSEMOTION) {
            mouse_x = ev.motion.x;
            mouse_y = ev.motion.y;
            mouse_motion = true;
        }
        else if(ev.type == SDL_MOUSEBUTTONDOWN) {
            mouse_down = true;
        }
        else if(mouse_motion && mouse_down) {
            break;
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
    if(!started) {
        // nothing clicked
        if(mouse_motion) {
            x_start = grid_snap(mouse_x + (GRID_STEP / 2)) - (POINT_W / 2);
            y_start = grid_snap(mouse_y + (GRID_STEP / 2)) - (POINT_H / 2);
        }
        if(mouse_down)
            started = true;
    }
    else if(!ended) {
        // clicked once; waiting to create a new side
        if(mouse_motion) {
            x_end = grid_snap(mouse_x + (GRID_STEP / 2)) - (POINT_W / 2);
            y_end = grid_snap(mouse_y + (GRID_STEP / 2)) - (POINT_H / 2);
        }
        if(mouse_down)
            ended = true;
    }
    else {
        m_done = true;
    }
}
