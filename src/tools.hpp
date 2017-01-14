#ifndef FIEFDOM_TOOLS_HPP
#define FIEFDOM_TOOLS_HPP

#include "types.hpp"
#include <SDL.h>

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

private:
    bool m_done;
    SDL_Texture* m_point;
};

#endif //FIEFDOM_TOOLS_HPP