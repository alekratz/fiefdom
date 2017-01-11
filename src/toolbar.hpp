#ifndef FIEFDOM_TOOLBAR_HPP
#define FIEFDOM_TOOLBAR_HPP

#include "types.hpp"
#include "util.hpp"
#include <functional>
#include <SDL.h>

class GameScene;

struct ToolbarItem;
using ToolbarItem_p = uptr<ToolbarItem>;

class Toolbar : public Entity {
    using this_t = Toolbar;
    using base_t = Entity;
public:
    using Callback_t = std::function<void(GameScene&, ToolbarItem&)>;
public:
    Toolbar(GameScene& game_scene);
    ~Toolbar() = default;

public:
    virtual void draw() override;
    virtual void update() override;


    /**
     * Adds an item to the toolbar, using the Windows style keyboard shortcut (i.e. &File, E&xit, etc.)
     */
    void add_item(cstref text, Callback_t callback);
private:
    GameScene& m_game_scene;
    //std::unordered_map<char, ToolbarItem_p> m_hotkey_items;
    vec<ToolbarItem_p> m_items;
};

using Toolbar_p = uptr<Toolbar>;

class ToolbarItem : public Entity, Loggable {
public:
    ToolbarItem(GameScene& game_scene, int32_t x_offset, int32_t y_offset, cstref name, Toolbar::Callback_t callback);
    virtual ~ToolbarItem();

public:
    /**
     * Gets the width in pixels of the toolbar item.
     */
    auto get_width() { 
        return m_width;
    }

    virtual void draw() override;
    virtual void update() override;
public:
    /** The text displayed by the toolbar item */
    str name;
    /** The function to call when this toolbar item is activated */
    Toolbar::Callback_t callback;
    /** The hotkey for activation, determined by the name */
    char hotkey;

    bool toggled;
private:
    int32_t m_width, m_height;
    int32_t x_offset, y_offset;
    GameScene& m_game_scene;

public:
    SDL_Texture* m_normal_texture;
    SDL_Texture* m_toggled_texture;
};

#endif
