#include "game_scene.hpp"
#include "globals.hpp"
#include <iostream>

constexpr auto START_MONEY = 1000;
constexpr auto MAX_DAYS = 28;
constexpr auto MAX_MONTH = 12;

/* Callback prototypes */
void building_mode_callback(GameScene&, ToolbarItem&);

GameScene::GameScene()
    : Loggable("GameScene")
    , mode(GameMode::None)
    , m_game_grid(std::make_unique<GameGrid>())
    , m_toolbar(std::make_unique<Toolbar>(*this))
    , m_money(START_MONEY)
    , m_month(3)
    , m_day(1) {
    m_serfs.push_back(std::make_unique<Serf>("Test", 0, 0));
    m_toolbar->add_item("&building", building_mode_callback);
    //m_toolbar->add_item("&select", test_callback);
}

void GameScene::draw() {
    for(auto& s : m_serfs)
        s->draw();
    m_game_grid->draw();
    
    m_toolbar->draw();
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
    }

    m_game_grid->update();
    for(auto& s : m_serfs)
        s->update();
    m_toolbar->update();
}

void building_mode_callback(GameScene& scene, ToolbarItem& item) {
    bool toggled = item.toggled;
    scene.m_toolbar->untoggle_all();
    if(toggled) {
        scene.mode = GameMode::None;
    }
    else {
        item.toggled = true;
        scene.mode = GameMode::Building;
    }
}