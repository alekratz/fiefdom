#ifndef FIEFDOM_SUBSCENES_HPP
#define FIEFDOM_SUBSCENES_HPP

#include "types.hpp"
#include <SDL.h>

class GameScene;

class Subscene : public Entity {
public:
    Subscene() : m_done(false) { }
    virtual bool done() { return m_done; }
protected:
    bool m_done;
};

template<typename T>
using Subscene_ptr = uptr<T>;
using Subscene_p = Subscene_ptr<Subscene>;

class PauseSubscene : public Subscene {
    using this_t = PauseSubscene;
public:
    PauseSubscene();
    ~PauseSubscene() = default;
private:

};

class YesNoSubscene : public Subscene {
    using this_t = YesNoSubscene;
public:
    YesNoSubscene(cstref prompt);
    ~YesNoSubscene();
public:
    virtual void draw() override;
    virtual void update() override;

    bool result() { return m_result; }
private:
    SDL_Rect m_draw_box;
    SDL_Texture* m_box_texture;
    bool m_result;
};

#endif //FIEFDOM_SUBSCENES_HPP
