#ifndef FIEFDOM_GAME_HPP
#define FIEFDOM_GAME_HPP

#include "types.hpp"
#include "util.hpp"
#include "game_scene.hpp"

bool create_sdl_window();

class Game : private Loggable {
    using this_t = Game;
public:
    Game();
    ~Game() {}

/* Operations */
public:
    void start();
private:
    void draw();
    void update();

/* Properties */
/* Members */
private:
    bool m_is_running;
    Entity_p m_game_scene;
};


#endif //FIEFDOM_GAME_HPP
