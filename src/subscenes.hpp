#ifndef FIEFDOM_SUBSCENES_HPP
#define FIEFDOM_SUBSCENES_HPP

#include "types.hpp"
#include <SDL.h>

class GameScene;

template<typename T>
using Subscene_ptr = uptr<T>;

class PauseSubscene : public Entity {
    using this_t = PauseSubscene;
public:
    PauseSubscene();
    ~PauseSubscene() = default;
private:

};

class YesNoSubscene : public Entity {
    using this_t = YesNoSubscene;
public:
    YesNoSubscene(cstref prompt);
    ~YesNoSubscene();
public:
    virtual void draw() override;
    virtual void update() override;

    bool result() { return m_result; }
    bool done() { return m_done; }
private:
    str m_prompt;
    SDL_Rect m_draw_box;
    SDL_Texture* m_box_texture;
    bool m_result;
    bool m_done;
};

using YesNoSubscene_p = Subscene_ptr<YesNoSubscene>;

#endif //FIEFDOM_SUBSCENES_HPP
