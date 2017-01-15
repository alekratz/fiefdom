#ifndef FIEFDOM_TOOLS_HPP
#define FIEFDOM_TOOLS_HPP

#include "types.hpp"
#include <SDL.h>

#define TOOL_POINT_W 10
#define TOOL_POINT_H 10

class DrawTool : public Entity {
    using this_t = DrawTool;
public:
    DrawTool();
    ~DrawTool();

public:
    virtual void draw() override;
    virtual void update() override;

public:
    bool done() const { return m_done; }
    void reset();

public:
    int32_t x_start, y_start;
    int32_t x_end, y_end;
    bool started, ended;
    int32_t w_min, h_min;
    int32_t w_max, h_max;
    double cost_per_unit;
    double cost_max;

private:
    bool m_done;
    SDL_Texture* m_cost_texture;
    SDL_Texture* m_size_texture;
};

#endif //FIEFDOM_TOOLS_HPP