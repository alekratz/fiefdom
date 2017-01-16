#include "subscenes.hpp"
#include "globals.hpp"
#include "util.hpp"
#include "game_grid.hpp"

#include <iostream>
using namespace std;

const auto YESNO_BOX_WIDTH = 360;
const auto YESNO_BOX_HEIGHT = 240;

const auto ADMIN_BOX_WIDTH = 360;
const auto ADMIN_BOX_HEIGHT = GAME_HEIGHT - 100;

const auto ADMIN_SERFS_PER_PAGE = 7;

/*
Right now, yes/no subscenes and administer subscene has(have) their own methods of drawing a UI box. In the future, I
want a ui.hpp header full of basic base classes that can do this for us. I don't want to have to worry about drawing the
UI on top of having to do the functionality of the subscene. 

This is definitely a good case for common code reuse and could probably have a positive impact on development time and
consistency.

However, I want to get this game done in time for the game jam, so we're just going to have to deal with copypasting for now.
 */

/*
    Pause subscene
*/
// PauseSubscene::PauseSubscene() : m_done(false) { }

/* 
    YesNo subscene   
*/
YesNoSubscene::YesNoSubscene(cstref prompt)
    : Entity()
    , m_draw_box({CENTER(GAME_WIDTH, YESNO_BOX_WIDTH), CENTER(GAME_HEIGHT, YESNO_BOX_HEIGHT), 
            YESNO_BOX_WIDTH, YESNO_BOX_HEIGHT})
    , m_done(false) {
        m_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STATIC | SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);
        SDL_SetRenderTarget(renderer, m_box_texture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_Rect outline {1, 1, GAME_WIDTH - 2, GAME_HEIGHT - 2};
        SDL_RenderDrawRect(renderer, &outline);
        SDL_SetRenderTarget(renderer, nullptr);

        auto prompt_surface = TTF_RenderText_Shaded(regular_font, prompt.c_str(), BLACK_OPAQUE, WHITE_OPAQUE);
        assert(prompt_surface);
        m_prompt_texture = SDL_CreateTextureFromSurface(renderer, prompt_surface);
        assert(m_prompt_texture);

        m_text_center.w = prompt_surface->w;
        m_text_center.h = prompt_surface->h;
        m_text_center.x = CENTER(GAME_WIDTH, m_text_center.w);
        m_text_center.y = CENTER(GAME_HEIGHT, m_text_center.h);

        SDL_FreeSurface(prompt_surface);

        auto yn_surface = TTF_RenderText_Shaded(regular_font, " Y / N ", WHITE_OPAQUE, BLACK_OPAQUE);
        assert(yn_surface);
        m_yn_texture = SDL_CreateTextureFromSurface(renderer, yn_surface);
        assert(m_yn_texture);

        m_yn_box.w = yn_surface->w;
        m_yn_box.h = yn_surface->h;
        m_yn_box.x = CENTER(GAME_WIDTH, m_yn_box.w);
        m_yn_box.y = m_text_center.y + 30;

        SDL_FreeSurface(yn_surface);
    }

YesNoSubscene::~YesNoSubscene() {
    SDL_DestroyTexture(m_box_texture);
    SDL_DestroyTexture(m_prompt_texture);
    SDL_DestroyTexture(m_yn_texture);
}

void YesNoSubscene::draw() { 
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, m_box_texture, nullptr, &m_draw_box);
    SDL_RenderCopy(renderer, m_prompt_texture, nullptr, &m_text_center);
    SDL_RenderCopy(renderer, m_yn_texture, nullptr, &m_yn_box);
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
        else if(ev.type == SDL_KEYDOWN && (ev.key.keysym.sym == SDLK_n || ev.key.keysym.sym == SDLK_ESCAPE)) {
            m_result = false;
            m_done = true;
        }
    }
}


void farm_callback(BuildSubscene&, ToolbarItem<BuildSubscene>&);
void cancel_callback(BuildSubscene&, ToolbarItem<BuildSubscene>&);

/*
    Build subscene
*/
BuildSubscene::BuildSubscene(vec<Entity_p>& buildings)
    : m_done(false)
    , m_toolbar(*this, 23) /* TODO : get rid of the magic number and base it off of toolbar dimensions */
    , m_build_mode(BuildingType::None)
    , m_buildings(buildings) {
    m_toolbar.add_item("&farm", farm_callback);
    m_toolbar.add_item("&cancel", cancel_callback);
}

void BuildSubscene::draw() {
    m_toolbar.draw();
    switch(m_build_mode) {
        case BuildingType::None: {
        } break;
        case BuildingType::Farm: {
            /* Draw farm drawing tools here */
            m_draw_tool.draw();
        } break;
    }
}

void BuildSubscene::update() {
    switch(m_build_mode) {
        case BuildingType::None: {
            m_toolbar.update();
        } break;
        case BuildingType::Farm: {
            /* Update farm drawing tools here */
            m_draw_tool.update();
            if(m_draw_tool.done()) {
                m_build_mode = BuildingType::None;
                SDL_Rect dims {
                    m_draw_tool.x_start + (TOOL_POINT_W / 2),
                    m_draw_tool.y_start + (TOOL_POINT_H / 2),
                    m_draw_tool.x_end - m_draw_tool.x_start,
                    m_draw_tool.y_end - m_draw_tool.y_start,
                };
                m_buildings.push_back(std::make_unique<Farm>(dims));
            }
        } break;
    }

    /* At any point, we can quit out of our current mode by pressing esc or c */
    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(ev.type == SDL_KEYDOWN && (ev.key.keysym.sym == SDLK_ESCAPE || ev.key.keysym.sym == SDLK_c)) {
            switch(m_build_mode) {
                case BuildingType::None: {
                    m_done = true;
                } break;
                default: {
                    m_toolbar.untoggle_all();
                    m_build_mode = BuildingType::None;
                } break;
            }
        }
    }
    
}

void farm_callback(BuildSubscene& subscene, ToolbarItem<BuildSubscene>& item) {
    item.toggled = true;
    subscene.m_build_mode = BuildingType::Farm;
    subscene.m_draw_tool.reset();
    subscene.m_draw_tool.w_min = subscene.m_draw_tool.h_min = 2 * GRID_STEP;
    subscene.m_draw_tool.w_max = subscene.m_draw_tool.h_max = 6 * GRID_STEP;
    subscene.m_draw_tool.cost_per_unit = 0.5;
}

void cancel_callback(BuildSubscene& subscene, ToolbarItem<BuildSubscene>&) {
    subscene.m_done = true;
}

/*
    Administer subscene
*/
AdministerSubscene::AdministerSubscene(const vec<Serf_p>& serfs) : Entity()
    , m_draw_box({10, CENTER(GAME_HEIGHT, ADMIN_BOX_HEIGHT), 
            ADMIN_BOX_WIDTH, ADMIN_BOX_HEIGHT})
    , m_done(false)
    , m_current_page(0)
    , m_serfs(serfs) {
    m_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STATIC | SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);
    SDL_SetRenderTarget(renderer, m_box_texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect outline {1, 1, GAME_WIDTH - 2, GAME_HEIGHT - 2};
    SDL_RenderDrawRect(renderer, &outline);
    SDL_SetRenderTarget(renderer, nullptr);
}

AdministerSubscene::~AdministerSubscene() {
    SDL_DestroyTexture(m_box_texture);
}

void AdministerSubscene::draw() {
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, m_box_texture, nullptr, &m_draw_box);
}

void AdministerSubscene::update() {
    /* At any point, we can quit out of our current mode by pressing esc or c */
    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    bool go_back = false, go_fwd = false;
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(ev.type == SDL_KEYDOWN) {
            switch(ev.key.keysym.sym) {
                case SDLK_ESCAPE: {
                    m_done = true;
                } break;
                case SDLK_q: {
                    go_back = true;
                } break;
                case SDLK_r: {
                    go_fwd = true;
                } break;
            }
        }

        // cancel each other out
        if(go_back && go_fwd)
            go_back = go_fwd = false;
        
        if(go_back && m_current_page > 0)
            m_current_page -= 1;
        else if(go_fwd && m_current_page < (page_count() - 1))
            m_current_page += 1;
        
        if(m_current_page >= page_count()) {
            // Because we're looking at a reference to the serf vec, it may change between when we last changed pages
            // and when the number of pages actually changed
            m_current_page = page_count() - 1;
        }
    }
}

int32_t AdministerSubscene::page_count() {
    return m_serfs.size() / ADMIN_SERFS_PER_PAGE;
}
