#ifndef FIEFDOM_SUBSCENES_HPP
#define FIEFDOM_SUBSCENES_HPP

#include "types.hpp"
#include "toolbar.hpp"
#include "buildings.hpp"
#include <SDL.h>

class GameScene;

template<typename T>
using Subscene_ptr = uptr<T>;

/**
 * PauseSubscene
 * Pauses the game. Unpauses when the "pause" key is hit.
 */
class PauseSubscene : public Entity {
    using this_t = PauseSubscene;
public:
    PauseSubscene();
    ~PauseSubscene() = default;

public:
    virtual void draw() override { } /* TODO : draw pause screen */
    virtual void update() override { } /* TODO : wait for pause hotkey */

    bool done() { return m_done; }
    void reset() { m_done = false; }

private:
    bool m_done;
};

/**
 * YesNoSubscene
 * Creates a yes/no window for the user to answer
 */
class YesNoSubscene : public Entity {
    using this_t = YesNoSubscene;
public:
    YesNoSubscene(cstref prompt);
    ~YesNoSubscene();
public:
    virtual void draw() override;
    virtual void update() override;

    bool result() { return m_result; }
    bool done() { return m_done; }
    void reset() { m_done = false; }
private:
    SDL_Rect m_draw_box;
    SDL_Texture* m_box_texture;
    SDL_Rect m_text_center;
    SDL_Texture* m_prompt_texture;
    SDL_Rect m_yn_box;
    SDL_Texture* m_yn_texture;
    bool m_result;
    bool m_done;
};

/**
 * BuildSubscene
 * Lets the user build shit.
 */
class BuildSubscene : public Entity {
    using this_t = BuildSubscene;
public:
    BuildSubscene();
    ~BuildSubscene() = default;

public:
    virtual void draw() override;
    virtual void update() override;

    bool done() const { return m_done; }
    void reset() { 
        m_done = false;
        m_build_mode = Building::None;
        m_toolbar.untoggle_all();
    }

private:
    bool m_done;
    Toolbar<this_t> m_toolbar;
    Building m_build_mode;

private:
    /*
    If you're wondering why I'm using friend methods, see the bottom of src/game_scene.hpp
    */
    friend void farm_callback(this_t&, ToolbarItem<this_t>&);
    friend void cancel_callback(this_t&, ToolbarItem<this_t>&);
};

#endif //FIEFDOM_SUBSCENES_HPP
