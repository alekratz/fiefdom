#ifndef FIEFDOM_BUILDINGS_HPP
#define FIEFDOM_BUILDINGS_HPP

#include "types.hpp"
#include <SDL.h>

class Farm : public Entity {
    using this_t = Farm;
    using base_t = Entity;
public:
    Farm(const SDL_Rect& dims);
    ~Farm() = default;

public:
    virtual void draw() override;
    virtual void update() override;

public:
    SDL_Rect dims;
    double yield_per_area;
};

#endif //FIEFDOM_BUILDINGS_HPP