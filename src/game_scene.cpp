#include "game_scene.hpp"
#include "globals.hpp"
#include "subscenes.hpp"
#include <iostream>

constexpr auto START_MONEY = 1000;
constexpr auto MAX_DAYS = 28;
constexpr auto MAX_MONTH = 12;

/* Callback prototypes */
void building_mode_callback(GameScene&, ToolbarItem<GameScene>&);
void quit_callback(GameScene&, ToolbarItem<GameScene>&);

GameScene::GameScene(bool& running)
    : Loggable("GameScene")
    , mode(GameMode::None)
    , m_running(running)
    , m_game_grid(std::make_unique<GameGrid>())
    , m_toolbar(*this)
    , m_money(START_MONEY)
    , m_month(3)
    , m_day(1)
    , m_building_subscene()
    , m_quit_subscene("Are you sure you want to quit?") {
    m_serfs.push_back(std::make_unique<Serf>("Test", 0, 0));
    m_toolbar.add_item("&building", building_mode_callback);
    m_toolbar.add_item("&quit", quit_callback);
    // m_toolbar.add_item("&select", test_callback);
}

void GameScene::draw() {
    SDL_SetRenderTarget(renderer, nullptr);
    for(auto& s : m_serfs)
        s->draw();
    m_game_grid->draw();
    m_toolbar.draw();

    /* Game-mode specific drawing */
    switch(mode) {
    case GameMode::None:
        {
            // Any non-subscene updates
        } break;
    case GameMode::Building:
        {
            m_building_subscene.draw();
        } break;
    case GameMode::Quitting:
        {
            m_quit_subscene.draw();
        } break;
    }
}

void GameScene::update() {
    auto generic_update = [&]() {
        m_game_grid->update();
        for(auto& s : m_serfs)
            s->update();
        m_toolbar.update();
    };

    switch(mode) {
    case GameMode::None:
        {
            generic_update();
        } break;
    case GameMode::Building:
        {
            m_game_grid->update();
            m_building_subscene.update();
            if(m_building_subscene.done()) {
                mode = GameMode::None;
                m_toolbar.untoggle_all();
                m_building_subscene.reset();
            }
        } break;
    case GameMode::Quitting:
        {
            m_quit_subscene.update();
            if(m_quit_subscene.done()) {
                m_running = !m_quit_subscene.result();
                m_quit_subscene.reset();
                mode = GameMode::None;
                m_toolbar.untoggle_all();
            }
        } break;
    }
}

void building_mode_callback(GameScene& scene, ToolbarItem<GameScene>& item) {
    bool toggled = item.toggled;
    scene.m_toolbar.untoggle_all();
    if(toggled) {
        scene.mode = GameMode::None;
    }
    else {
        item.toggled = true;
        scene.mode = GameMode::Building;
    }
}

void quit_callback(GameScene& game_scene, ToolbarItem<GameScene>& item) {
    if(game_scene.mode != GameMode::Quitting) {
        item.toggled = true;
        game_scene.mode = GameMode::Quitting;
    }
}
