#ifndef FIEFDOM_GAME_SCENE_HPP
#define FIEFDOM_GAME_SCENE_HPP

#include "types.hpp"
#include "util.hpp"
#include "game_grid.hpp"
#include "serf.hpp"
#include "toolbar.hpp"
#include "subscenes.hpp"

class Game;

enum class GameMode {
    None,
    Administering,
    Building,
    Quitting,
};

class GameScene : public Entity, Loggable {
/* typedefs */
    using this_t = GameScene;
    using base_t = Entity;
/* ctor/dtor */
public:
    GameScene(bool& running);
    ~GameScene() = default;

/* Operations */
public:
    virtual void draw() override;
    virtual void update() override;

/* Members */
public:
    GameMode mode;
private:
    bool& m_running;
    GameGrid_p m_game_grid;
    Toolbar<this_t> m_toolbar;
    vec<Serf_p> m_serfs;
    vec<Entity_p> m_buildings;
    int32_t m_month;
    int32_t m_day;
    AdministerSubscene m_administer_subscene;
    BuildSubscene m_building_subscene;
    YesNoSubscene m_quit_subscene;
/* Friends */
private:
    /*
    The reason that callbacks are friend functions of this type is because they require private-level access to the 
    GameScene object. While a class method would be the obvious choice in this instance, class methods cannot be
    passed around like std::function, because it is not a static member; i.e., its existence depends on the existence
    of a GameScene object, which the compiler cannot guarantee is the case.

    While you and I are aware that a ToolbarItem object may not exist without a GameScene object (which is functionally
    a singleton), we can just as easily violate these rules with the compiler being completely unaware that we did that,
    and that's not good eats.

    You will see this pattern anytime we deal with callbacks; get used to it.
    */
    friend void administer_mode_callback(GameScene&, ToolbarItem<GameScene>&);
    friend void building_mode_callback(GameScene&, ToolbarItem<this_t>&);
    friend void quit_callback(GameScene&, ToolbarItem<this_t>&);
};

using GameScene_p = uptr<GameScene>;

#endif //FIEFDOM_GAME_SCENE_HPP
