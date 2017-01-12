#include "game_scene.hpp"
#include "globals.hpp"
#include "subscenes.hpp"
#include <iostream>

constexpr auto START_MONEY = 1000;
constexpr auto MAX_DAYS = 28;
constexpr auto MAX_MONTH = 12;

/* Callback prototypes */
void building_mode_callback(GameScene&, ToolbarItem&);
void quit_callback(GameScene&, ToolbarItem&);

GameScene::GameScene()
    : Loggable("GameScene")
    , mode(GameMode::None)
    , m_game_grid(std::make_unique<GameGrid>())
    , m_toolbar(std::make_unique<Toolbar>(*this))
    , m_money(START_MONEY)
    , m_month(3)
    , m_day(1)
    , m_building_subscene(nullptr)
    , m_quit_subscene(nullptr) {
    m_serfs.push_back(std::make_unique<Serf>("Test", 0, 0));
    m_toolbar->add_item("&building", building_mode_callback);
    m_toolbar->add_item("&quit", quit_callback);
    // m_toolbar->add_item("&select", test_callback);
}

void GameScene::draw() {
    SDL_SetRenderTarget(renderer, nullptr);
    for(auto& s : m_serfs)
        s->draw();
    m_game_grid->draw();
    m_toolbar->draw();

    /* Game-mode specific drawing */
    switch(mode) {
    case GameMode::None:
        {
            // Any non-subscene updates
        } break;
    case GameMode::Building:
        {
            m_building_subscene->draw();
        } break;
    case GameMode::Quitting:
        {
            m_quit_subscene->draw();
        } break;
    }
}

void GameScene::update() {
    auto generic_update = [&]() {
        m_game_grid->update();
        for(auto& s : m_serfs)
            s->update();
        m_toolbar->update();
    };

    switch(mode) {
    case GameMode::None:
        {
            generic_update();
            // Any non-subscene updates
        } break;
    case GameMode::Building:
        {
            generic_update();
            m_building_subscene->update();
        } break;
    case GameMode::Quitting:
        {
            m_quit_subscene->update();
        } break;
    }
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

void quit_callback(GameScene& game_scene, ToolbarItem& item) {
    // at this point the quit callback does the equivalent of saying "I'M TELLING MY DAD ON YOU!"
    item.toggled = true;
    game_scene.m_quit_subscene = std::make_unique<YesNoSubscene>();

}
