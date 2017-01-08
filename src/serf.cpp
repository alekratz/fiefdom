#include "serf.hpp"
#include "util.hpp"
#include "globals.hpp"
#include <SDL.h>

// How much happiness is lost in one day
constexpr auto HAPPINESS_DECAY = 1;
// How much happiness a serf starts with
constexpr auto HAPPINESS_START = 50;
// How much happiness a serf can possibly have
constexpr auto HAPPINESS_CAP = 100;
// How much motivation a serf starts with
constexpr auto MOTIVATION_START = 50;
// How much motivation a serf can possibly have
constexpr auto MOTIVATION_CAP = 100;
// How much motivation a serf can possibly have when they are depressed
constexpr auto DEPRESSED_MOTIVATION_CAP = 25;
// How much net happiness must be earned before depression goes away
constexpr auto DEPRESSED_MOTIVATION_GOAL = 10;
// How much production a serf starts with
constexpr auto PRODUCTION_START = 50;
// How much production a serf can possibly have
constexpr auto PRODUCTION_CAP = 100;
// Number of days in a row that a serf is sad before they become depressed
constexpr auto DEPRESSION_STREAK = 7;
// Multiplier for the amount of motivation lost when a serf is depressed.
constexpr auto DEPRESSED_MOTIVATION_LOSS_FACTOR = 3;

constexpr auto SERF_WIDTH = 10;
constexpr auto SERF_HEIGHT = 10;

Serf::Serf(const str& name, int32_t x_pos, int32_t y_pos)
    : m_name(name)
    , m_happiness(HAPPINESS_START)
    , m_motivation(MOTIVATION_START)
    //, m_production(PRODUCTION_START)
    , m_last_happiness(m_happiness)
    // , m_last_motivation(m_motivation)
    // , m_last_production(m_production)
    , m_sad_streak(0)
    , m_depressed(false)
    , m_x_pos(x_pos)
    , m_y_pos(y_pos)
{ }

void Serf::set_happiness(int32_t value) {
    m_happiness = (value > HAPPINESS_CAP) ? HAPPINESS_CAP : value;
}

void Serf::day() {
    /*
     * A serf loses one point of happiness per day, as a fixed rate.
     * Happiness has a certain cap; thus, a serf cannot forever gain happiness points.
     * If a serf has consistently lost or not gained happiness over the course of 7 days, their motivation loss triples.
       During this period, they are depressed. Also, motivation is capped at a hard limit.
     * Depression can strike if a serf's happiness is below a certain threshold (TODO: how much?).
     * To kick a serf out of depression, their happiness must rise a net of 10 points since they became depressed.
     * If they are depressed for long enough, they will stop eating and happiness income will be halved.
       * TODO: how long?
     * A serf's motivation is tied to their happiness. If a serf has gained happiness that day, then they will gain one
       motivation point. If they lost more than one point of happiness that day, then they will lose one motivation
       point.
     */

    // Happiness calculation
    m_happiness -= HAPPINESS_DECAY;

    // Motivation and depression calculation
    if(m_happiness <= m_last_happiness) {
        m_sad_streak += 1;
        int multiplier = m_depressed ? DEPRESSED_MOTIVATION_LOSS_FACTOR : 1;
        m_motivation -= 1 * multiplier;
    }
    else {
        m_sad_streak = 0;
        m_motivation += 1;
        int cap = m_depressed ? DEPRESSED_MOTIVATION_CAP : MOTIVATION_CAP;
        if(m_motivation > cap)
            m_motivation = cap;
    }

    if(m_depressed)
        m_depressed = m_happiness < m_depression_goal;
    else if(m_sad_streak >= DEPRESSION_STREAK) {
        m_depressed = true;
        m_depression_goal = m_happiness + DEPRESSED_MOTIVATION_GOAL;
    }
    // TODO : write tests for this
}

void Serf::draw() {
    SDL_Rect pos {  CENTER(m_x_pos, SERF_WIDTH) + GAME_WIDTH / 2 + x_offset,
                    CENTER(m_y_pos, SERF_HEIGHT) + GAME_HEIGHT / 2 + y_offset, 
                    SERF_WIDTH, 
                    SERF_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &pos);
    SDL_SetRenderDrawColor(renderer, 0x6E, 0x4D, 0x07, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &pos);
}

void Serf::update() {
    // AI here
}