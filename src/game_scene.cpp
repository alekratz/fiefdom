#include "game_scene.hpp"
#include "globals.hpp"

constexpr auto START_MONEY = 1000;
constexpr auto MAX_DAYS = 28;
constexpr auto MAX_MONTH = 12;

GameScene::GameScene()
    : Loggable("GameScene")
    , m_game_grid(std::make_unique<GameGrid>())
    , m_money(START_MONEY)
    , m_month(3)
    , m_day(1)
    , m_mode(GameMode::None) {
    m_serfs.push_back(std::make_unique<Serf>("Test", 0, 0));
    
}

void GameScene::draw() {
    for(auto& s : m_serfs)
        s->draw();
    m_game_grid->draw();
}

void GameScene::update() {
    constexpr auto GRID_MOVE_SPEED = 0.05;
    constexpr auto EVENT_SZ = 128; // arbitrary event size
    static SDL_Event evs[EVENT_SZ]; 
    int count = SDL_PeepEvents(evs, EVENT_SZ, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    for(int i = 0; i < count; i++) {
        auto ev = evs[i];
        if(ev.type == SDL_MOUSEMOTION) {
            x_offset = (ev.motion.x - (GAME_WIDTH / 2)) * GRID_MOVE_SPEED;
            y_offset = (ev.motion.y - (GAME_HEIGHT / 2)) * GRID_MOVE_SPEED;
        }
        else if(ev.type == SDL_KEYDOWN) {
            // TODO : move this to its own "game scene keyboard handler" object or something
            switch(ev.key.keysym.sym) {
            case SDLK_b:
                if(!ev.key.repeat) {
                    m_mode = (m_mode == GameMode::Building) ? GameMode::None : GameMode::Building;
                    m_logger->debug("Toggling gamemode between building");
                }
                break;
            }
        }
    }

    m_game_grid->update();
    for(auto& s : m_serfs)
        s->update();
}