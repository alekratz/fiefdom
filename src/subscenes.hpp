#ifndef FIEFDOM_SUBSCENES_HPP
#define FIEFDOM_SUBSCENES_HPP

#include "types.hpp"
#include "toolbar.hpp"
#include "tools.hpp"
#include "buildings.hpp"
#include "serf.hpp"
#include <SDL.h>

class GameScene;

template<typename T>
using Subscene_ptr = uptr<T>;

/**
 * PauseSubscene
 * Pauses the game. Unpauses when the "pause" key is hit.
 *
class PauseSubscene : public Entity {
    using this_t = PauseSubscene;
public:
    PauseSubscene();
    ~PauseSubscene() = default;

public:
    void draw() override { }
    void update() override { }

    bool done() { return m_done; }
    void reset() { m_done = false; }

private:
    bool m_done;
};
*/

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
    void draw() override;
    void update() override;

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
    BuildSubscene(vec<Entity_p>& buildings);
    ~BuildSubscene() = default;

public:
    void draw() override;
    void update() override;

    bool done() const { return m_done; }
    void reset() { 
        m_done = false;
        m_build_mode = BuildingType::None;
        m_toolbar.untoggle_all();
    }

private:
    bool m_done;
    Toolbar<this_t> m_toolbar;
    BuildingType m_build_mode;
    DrawTool m_draw_tool;
    vec<Entity_p>& m_buildings;

private:
    /*
    If you're wondering why I'm using friend methods, see the bottom of src/game_scene.hpp
    */
    friend void farm_callback(this_t&, ToolbarItem<this_t>&);
    friend void cancel_callback(this_t&, ToolbarItem<this_t>&);
};

/**
 * AdministerSubscene
 * Lets the user administer serfs.
 */
class AdministerSubscene : public Entity {
public:
    AdministerSubscene(const vec<Serf_p>& serfs);
    ~AdministerSubscene();

public:
    void draw() override;
    void update() override;

    bool done() const { return m_done; }
    void reset() { m_done = false; }

private:
    int32_t page_count();

private:
    SDL_Rect m_draw_box;
    SDL_Texture* m_box_texture;
    bool m_done;
    int32_t m_current_page;
    const vec<Serf_p>& m_serfs;
};

#endif //FIEFDOM_SUBSCENES_HPP
