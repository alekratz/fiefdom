#ifndef FIEFDOM_GAME_SCENE_HPP
#define FIEFDOM_GAME_SCENE_HPP

#include "types.hpp"
#include "game_grid.hpp"
#include "serf.hpp"

enum class GameMode {
    None,
    Building,
};

class GameScene : public Entity {
    using this_t = GameScene;
    using base_t = Entity;
public:
    GameScene();
    ~GameScene() = default;

public:
    virtual void draw() override;
    virtual void update() override;

private:
    GameGrid_p m_game_grid;
    vec<Serf_p> m_serfs;
    int32_t m_money;
    int32_t m_month;
    int32_t m_day;
    GameMode m_mode;
};

using GameScene_p = uptr<GameScene>;

#endif //FIEFDOM_GAME_SCENE_HPP