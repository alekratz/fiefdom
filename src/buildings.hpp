#ifndef FIEFDOM_BUILDINGS_HPP
#define FIEFDOM_BUILDINGS_HPP

#include "types.hpp"
#include <SDL.h>

enum class BuildingType {
    None,
    Farm,
};

class Farm : public Entity {
    using this_t = Farm;
    using base_t = Entity;
public:
    Farm(const SDL_Rect& dims);
    ~Farm();

public:
    void draw() override;
    void update() override;

public:
    SDL_Rect dims;
    double yield_per_area;

private:
    SDL_Texture* m_text_texture;
};

using Farm_p = Entity_ptr<Farm>;

#endif //FIEFDOM_BUILDINGS_HPP
