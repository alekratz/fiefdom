#include "toolbar.hpp"
#include "game_scene.hpp"
#include "globals.hpp"
#include <SDL_ttf.h>
#include <SDL.h>
#include <cassert>

constexpr auto TOOLBAR_HEIGHT = 23;
constexpr auto TOOLBAR_ITEM_VPADDING = 3;
constexpr auto TOOLBAR_ITEM_HPADDING = 13;
//const SDL_Color WHITE { 255, 255, 255, SDL_ALPHA_OPAQUE };
// const SDL_Color BLACK { 0, 0, 0, SDL_ALPHA_OPAQUE };

template<typename CallbackT>
Toolbar<CallbackT>::Toolbar(CallbackT& game_state, int32_t y_offset) 
    : m_game_state(game_state)
    , m_x_offset(TOOLBAR_ITEM_HPADDING)
    , m_y_offset(y_offset) { }

template<typename CallbackT>
void Toolbar<CallbackT>::untoggle_all() {
    for(auto& t : m_items) {
        t->toggled = false;
    }
}

template<typename CallbackT>
void Toolbar<CallbackT>::draw() {
    // Base toolbar
    SDL_Rect bar { -1, m_y_offset, GAME_WIDTH + 2, TOOLBAR_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &bar);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &bar);
    
    // Toolbar items
    for(auto& t : m_items) {
        t->draw();
    }
}

template<typename CallbackT>
void Toolbar<CallbackT>::update() {

    for(auto& t : m_items) {
        t->update();
    }
}

template<typename CallbackT>
void Toolbar<CallbackT>::add_item(cstref text, Callback_t callback) {
    auto new_item = std::make_unique<ToolbarItem<CallbackT>>(m_game_state, m_x_offset, TOOLBAR_ITEM_VPADDING + m_y_offset, text, callback);
    m_x_offset += new_item->get_width() + TOOLBAR_ITEM_HPADDING;
    m_items.push_back(std::move(new_item));
}

template<typename CallbackT>
ToolbarItem<CallbackT>::ToolbarItem(CallbackT& game_state, int32_t x_offset, int32_t y_offset, cstref name, typename Toolbar<CallbackT>::Callback_t callback)
    : Loggable("ToolbarItem")
    , name(name)
    , callback(callback)
    , hotkey(0)
    , toggled(false)
    , x_offset(x_offset)
    , y_offset(y_offset)
    , m_game_state(game_state)
    , m_normal_texture(nullptr)
    , m_toggled_texture(nullptr) {

    auto index = name.find("&");
    if(index != str::npos) {
        assert(index <= (name.size() - 2) && "& must come before a character in ToolbarItem");
        this->name.erase(index, 1);
    }
    // Create textures for this item so we don't have to do these weird calculations every frame
    TTF_SizeText(regular_font, this->name.c_str(), &m_width, &m_height);
    m_normal_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STATIC | SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    assert(m_normal_texture && "Could not create texture");
    m_toggled_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC | SDL_TEXTUREACCESS_TARGET, m_width, m_height);
    assert(m_toggled_texture && "Could not create texture");

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_TRANSPARENT);
    SDL_SetRenderTarget(renderer, m_normal_texture);
    SDL_RenderClear(renderer);

    // Draw the text on each, and highlight the hotkey if necessary
    SDL_Surface* text_surface = nullptr;
    SDL_Texture* text_texture = nullptr;

    if(index != str::npos) {
        /* normal texture */
        text_surface = TTF_RenderText_Blended(regular_font, this->name.c_str(), SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE });
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect draw_rect { 0, 0, m_width, m_height };
        SDL_RenderCopy(renderer, text_texture, nullptr, &draw_rect);
        assert(text_surface && "Could not render font");
        hotkey = this->name[index];
        auto hotkey_str = str() + hotkey;
        /* the way we figure out where to blit the hotkey is to get the length of the string before the hotkey, and
           then blit starting from there */
        auto pre = this->name.substr(0, index);
        int32_t pre_width;
        TTF_SizeText(regular_font, pre.c_str(), &pre_width, nullptr);
        int32_t hotkey_w;
        TTF_SizeText(regular_font, hotkey_str.c_str(), &hotkey_w, nullptr);
        draw_rect.x = pre_width;
        draw_rect.w = hotkey_w;
        draw_rect.h = m_height;
        auto hotkey_surface = TTF_RenderText_Shaded(regular_font, (str() + hotkey).c_str(), WHITE_OPAQUE, BLACK_OPAQUE);
        auto hotkey_texture = SDL_CreateTextureFromSurface(renderer, hotkey_surface);
        SDL_RenderCopy(renderer, hotkey_texture, nullptr, &draw_rect);
        SDL_FreeSurface(hotkey_surface);
        SDL_DestroyTexture(hotkey_texture);

        /* toggled texture */
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
        text_surface = TTF_RenderText_Shaded(regular_font, this->name.c_str(), WHITE_OPAQUE, BLACK_OPAQUE);
        SDL_SetRenderTarget(renderer, m_toggled_texture);
        SDL_RenderClear(renderer);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        draw_rect = { 0, 0, m_width, m_height };
        SDL_RenderCopy(renderer, text_texture, nullptr, &draw_rect);
        draw_rect.x = pre_width;
        draw_rect.w = hotkey_w;
        draw_rect.h = m_height;
        hotkey_surface = TTF_RenderText_Shaded(regular_font, (str() + hotkey).c_str(), BLACK_OPAQUE, WHITE_OPAQUE);
        hotkey_texture = SDL_CreateTextureFromSurface(renderer, hotkey_surface);
        SDL_RenderCopy(renderer, hotkey_texture, nullptr, &draw_rect);
        SDL_FreeSurface(hotkey_surface);
        SDL_DestroyTexture(hotkey_texture);
    }
    else {
        text_surface = TTF_RenderText_Blended(regular_font, this->name.c_str(), SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE });
        assert(text_surface && "Could not render font");
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_RenderCopy(renderer, text_texture, nullptr, nullptr);

        /* toggled texture */
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
        text_surface = TTF_RenderText_Shaded(regular_font, this->name.c_str(), WHITE_OPAQUE, BLACK_OPAQUE);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect draw_rect { 0, 0, m_width, m_height };
        SDL_RenderCopy(renderer, text_texture, nullptr, &draw_rect);
    }
    assert(m_normal_texture && "Default texture was not created");
    
    assert(text_surface);
    SDL_FreeSurface(text_surface);
    assert(text_texture);
    SDL_DestroyTexture(text_texture);
    SDL_SetRenderTarget(renderer, nullptr);
}

template<typename CallbackT>
ToolbarItem<CallbackT>::~ToolbarItem() {
    SDL_DestroyTexture(m_normal_texture);
    SDL_DestroyTexture(m_toggled_texture);
}

template<typename CallbackT>
void ToolbarItem<CallbackT>::draw() {
    SDL_SetRenderTarget(renderer, nullptr);
    auto texture = toggled ? m_toggled_texture : m_normal_texture;
    SDL_Rect rect { x_offset, y_offset, m_width, m_height };
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

template<typename CallbackT>
void ToolbarItem<CallbackT>::update() {
    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(hotkey != 0 && ev.type == SDL_KEYDOWN && ev.key.keysym.sym == hotkey && !ev.key.repeat)
            callback(m_game_state, *this);
        else if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
            SDL_Rect mouse_rect = { ev.button.x, ev.button.y, 1, 1 };
            SDL_Rect text_rect = { x_offset, y_offset, m_width, m_height };
            SDL_Rect result;
            if(SDL_IntersectRect(&mouse_rect, &text_rect, &result) == SDL_TRUE) {
                callback(m_game_state, *this);
            }
        }
    }
}
