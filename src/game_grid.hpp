#ifndef FIEFDOM_GAME_GRID_HPP
#define FIEFDOM_GAME_GRID_HPP

#include "types.hpp"
#include <SDL.h>

class GameGrid : public Entity {
private:
    using this_t = GameGrid;
    using base_t = Entity;
public:
    GameGrid();
    ~GameGrid();

public:
    virtual void draw() override;
    virtual void update() override;
};

using GameGrid_p = uptr<GameGrid>;

#endif // FIEFDOM_GAME_GRID_HPP