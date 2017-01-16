#ifndef FIEFDOM_TOOLBAR_HPP
#define FIEFDOM_TOOLBAR_HPP

#include "types.hpp"
#include "util.hpp"
#include <functional>
#include <SDL.h>

//class GameScene;

template<typename CallbackT>
class ToolbarItem;
template<typename CallbackT>
using ToolbarItem_p = uptr<ToolbarItem<CallbackT>>;

template<typename CallbackT>
class Toolbar : public Entity {
    using this_t = Toolbar;
    using base_t = Entity;
public:
    using Callback_t = std::function<void(CallbackT&, ToolbarItem<CallbackT>&)>;

public:
    Toolbar(CallbackT& game_state, int32_t y_offset=0);
    ~Toolbar() = default;

public:
    /* Untoggles all toolbar items */
    void untoggle_all();
    void draw() override;
    void update() override;

    /**
     * Adds an item to the toolbar, using the Windows style keyboard shortcut (i.e. &File, E&xit, etc.)
     */
    void add_item(cstref text, Callback_t callback);
private:
    CallbackT& m_game_state;
    //std::unordered_map<char, ToolbarItem_p> m_hotkey_items;
    vec<ToolbarItem_p<CallbackT>> m_items;
    int32_t m_x_offset;
    int32_t m_y_offset;
};

template<typename CallbackT>
using Toolbar_p = uptr<Toolbar<CallbackT>>;

template<typename CallbackT>
class ToolbarItem : public Entity, Loggable {
public:
    ToolbarItem(CallbackT& game_scene, int32_t x_offset, int32_t y_offset, cstref name, typename Toolbar<CallbackT>::Callback_t callback);
    virtual ~ToolbarItem();

public:
    /**
     * Gets the width in pixels of the toolbar item.
     */
    auto get_width() { 
        return m_width;
    }

    void draw() override;
    void update() override;
public:
    /** The text displayed by the toolbar item */
    str name;
    /** The function to call when this toolbar item is activated */
    typename Toolbar<CallbackT>::Callback_t callback;
    /** The hotkey for activation, determined by the name */
    char hotkey;

    bool toggled;
private:
    int32_t m_width, m_height;
    int32_t x_offset, y_offset;
    CallbackT& m_game_state;

public:
    SDL_Texture* m_normal_texture;
    SDL_Texture* m_toggled_texture;
};

/* Forward declarations of toolbar and toolbaritem template classes */
class GameScene;
template class Toolbar<GameScene>;
template class ToolbarItem<GameScene>;
class BuildSubscene;
template class Toolbar<BuildSubscene>;
template class ToolbarItem<BuildSubscene>;
#endif
