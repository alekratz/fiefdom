#include "tools.hpp"
#include "globals.hpp"
#include "game_grid.hpp"
#include "util.hpp"
#include <cassert>
#include <cmath>
#include <SDL.h>
#include <spdlog/fmt/fmt.h>


const SDL_Color TEXT_FG = BLACK_OPAQUE;
const SDL_Color TEXT_BG { 128, 128, 128, 128 };

// TODO : all x_end - x_start and y_end - y_start need to be wrapped with abs(), because they need to be magnitudes.

DrawTool::DrawTool()
    : x_start(-TOOL_POINT_W)
    , y_start(-TOOL_POINT_H)
    , x_end(-TOOL_POINT_W)
    , y_end(-TOOL_POINT_H)
    , started(false) 
    , ended(false)
    , w_min(0), h_min(0)
    , w_max(0), h_max(0)
    , cost_per_unit(0.0)
    , m_done(false)
    , m_cost_texture(nullptr)
    , m_size_texture(nullptr) { }

DrawTool::~DrawTool() { }

void DrawTool::reset() {
    started = ended = m_done = false;
    x_start = x_end = -TOOL_POINT_W;
    y_start = y_end = -TOOL_POINT_H;
    if(m_cost_texture != nullptr) {
        SDL_DestroyTexture(m_cost_texture);
        m_cost_texture = nullptr;
    }
    if(m_size_texture != nullptr) {
        SDL_DestroyTexture(m_size_texture);
        m_size_texture = nullptr;
    }
}

void DrawTool::draw() {
    SDL_SetRenderDrawColor(renderer, 100, 128, 255, 128);
    bool valid = true;
    if(started) {
        if(w_min > 0 && ((x_end - x_start) < w_min))
            valid = false;
        else if(w_max > 0 && ((x_end - x_start) > w_max))
            valid = false;
        else if(h_min > 0 && ((y_end - y_start) < h_min))
            valid = false;
        else if(h_max > 0 && ((y_end - y_start) > h_max))
            valid = false;
    }

    if(!valid){
        SDL_SetRenderDrawColor(renderer, 214, 50, 48, 128);
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_Rect start_rect { x_start + x_offset, y_start + y_offset, TOOL_POINT_W, TOOL_POINT_H };
    SDL_RenderFillRect(renderer, &start_rect);

    if (started) {
        // clicked once; waiting to create a new side
        //SDL_SetRenderDrawColor(renderer, 100, 128, 255, 128);
        SDL_Rect outline {
            x_start + x_offset + (TOOL_POINT_W / 2),
            y_start + y_offset + (TOOL_POINT_H / 2),
            x_end - x_start - 1,
            y_end - y_start - 1,
        };
        SDL_RenderDrawRect(renderer, &outline);
        SDL_RenderFillRect(renderer, &outline);
        SDL_Rect end_rect { x_end + x_offset, y_end + y_offset, TOOL_POINT_W, TOOL_POINT_H };
        SDL_Rect aux_rect1 { x_start + x_offset, y_end + y_offset, TOOL_POINT_W, TOOL_POINT_H };
        SDL_Rect aux_rect2 { x_end + x_offset, y_start + y_offset, TOOL_POINT_W, TOOL_POINT_H };
        SDL_RenderFillRect(renderer, &end_rect);
        SDL_RenderFillRect(renderer, &aux_rect1);
        SDL_RenderFillRect(renderer, &aux_rect2);

        if(m_cost_texture != nullptr) {
            int32_t text_w, text_h;
            SDL_QueryTexture(m_cost_texture, nullptr, nullptr, &text_w, &text_h);
            SDL_Rect draw_rect { x_offset + x_start + CENTER(x_end - x_start, text_w), y_offset + y_start + text_h + CENTER(y_end - y_start, text_h), text_w, text_h };
            SDL_RenderCopy(renderer, m_cost_texture, nullptr, &draw_rect);
        }
        if(m_size_texture != nullptr) {
            int32_t text_w, text_h;
            SDL_QueryTexture(m_size_texture, nullptr, nullptr, &text_w, &text_h);
            SDL_Rect draw_rect { x_offset + x_start + CENTER(x_end - x_start, text_w), y_offset + y_start + CENTER(y_end - y_start, text_h), text_w, text_h };
            SDL_RenderCopy(renderer, m_size_texture, nullptr, &draw_rect);
        }
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

    /*
        Start/end point placement
    */
    if(!started) {
        // nothing clicked
        if(mouse_motion) {
            x_start = grid_snap(mouse_x + (GRID_STEP / 2)) - (TOOL_POINT_W / 2);
            y_start = grid_snap(mouse_y + (GRID_STEP / 2)) - (TOOL_POINT_H / 2);
        }
        if(mouse_down)
            started = true;
    }
    else if(!ended) {
        // clicked once; waiting to create a new side
        if(mouse_motion) {
            x_end = grid_snap(mouse_x + (GRID_STEP / 2)) - (TOOL_POINT_W / 2);
            y_end = grid_snap(mouse_y + (GRID_STEP / 2)) - (TOOL_POINT_H / 2);
        }
        if(mouse_down)
            ended = true;
        
        /*
            Text position calculations
        */
        if(mouse_motion) {
            if(m_size_texture != nullptr) {
                SDL_DestroyTexture(m_size_texture);
                m_size_texture = nullptr;
            }
            assert(m_size_texture == nullptr);
            auto size_str = fmt::format("{} x {}", (x_end - x_start) / GRID_STEP, (y_end - y_start) / GRID_STEP);
            auto size_surface = TTF_RenderText_Shaded(regular_font, size_str.c_str(), TEXT_FG, TEXT_BG);
            m_size_texture = SDL_CreateTextureFromSurface(renderer, size_surface);
        }

        if(cost_per_unit != 0.0 && mouse_motion) {
            // TODO : figure out how to speed this one up
            if(m_cost_texture != nullptr) {
                SDL_DestroyTexture(m_cost_texture);
                m_cost_texture = nullptr;
            }
            assert(m_cost_texture == nullptr);
            double cost = ((abs(x_end - x_start) / GRID_STEP * abs(y_end - y_start) / GRID_STEP)) * cost_per_unit;
            auto cost_str = fmt::format("{} " CURRENCY, cost);
            auto cost_surface = TTF_RenderText_Shaded(regular_font, cost_str.c_str(), TEXT_FG, TEXT_BG);
            m_cost_texture = SDL_CreateTextureFromSurface(renderer, cost_surface);
        }
    }
    else {
        m_done = true;
    }
}
