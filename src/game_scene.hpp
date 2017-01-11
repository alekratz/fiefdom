#ifndef FIEFDOM_GAME_SCENE_HPP
#define FIEFDOM_GAME_SCENE_HPP

#include "types.hpp"
#include "util.hpp"
#include "game_grid.hpp"
#include "serf.hpp"
#include "toolbar.hpp"

enum class GameMode {
    None,
    Building,
};

class GameScene : public Entity, Loggable {
    using this_t = GameScene;
    using base_t = Entity;
public:
    GameScene();
    ~GameScene() = default;

public:
    virtual void draw() override;
    virtual void update() override;

    GameMode mode;
private:
    GameGrid_p m_game_grid;
    Toolbar_p m_toolbar;
    vec<Serf_p> m_serfs;
    int32_t m_money;
    int32_t m_month;
    int32_t m_day;
};

using GameScene_p = uptr<GameScene>;

#endif //FIEFDOM_GAME_SCENE_HPP