#ifndef FIEFDOM_GAME_GRID_HPP
#define FIEFDOM_GAME_GRID_HPP

#define GRID_STEP 32

#include "types.hpp"
#include <SDL.h>

/**
 * Snaps a coordinate to the closest GRID_STEP multiple.
 */
inline int32_t grid_snap(int32_t in) {
    return (in + GRID_STEP / 32) - (in % GRID_STEP);
}

class GameGrid : public Entity {
private:
    using this_t = GameGrid;
    using base_t = Entity;
public:
    GameGrid();
    ~GameGrid();

public:
    void draw() override;
    void update() override;
};

using GameGrid_p = uptr<GameGrid>;

#endif // FIEFDOM_GAME_GRID_HPP
