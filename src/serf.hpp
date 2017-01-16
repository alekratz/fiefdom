#ifndef FIEFDOM_SERF_HPP
#define FIEFDOM_SERF_HPP

#include "types.hpp"
#include <cstdint>

class Serf : public Entity {
    using this_t = Serf;
public:
    /**
     * Creates a new instance of a serf.
     * @param name the serf's name. This is the actual display name, so it should be something creative.
     */
    Serf(cstref name, int32_t x_pos, int32_t y_pos);
    ~Serf() { }

    /* Operations */
public:
    /**
     * Advances the serf's state forward one day, based on the current values and the values before
     */
    void day();

    virtual void draw() override;
    virtual void update() override;

    /* Properties */
public:
    cstref name() const { return m_name; }
    int32_t happiness() const { return m_happiness; }
    void set_happiness(int32_t value);

    /* Members */
private:
    // Name of the serf
    str m_name;
    // Happiness the serf currently has
    int32_t m_happiness;
    // Motivation the serf currently has
    int32_t m_motivation;
    // Production the serf currently has
    //int32_t m_production;

    // Happiness the serf had yesterday
    int32_t m_last_happiness;
    // Motivation the serf had yesterday
    // int32_t m_last_motivation;
    // Production the serf had yesterday
    // int32_t m_last_production;

    int32_t m_sad_streak;
    bool m_depressed;
    // How many points of happiness this serf needs to accrue to stop being depressed
    int32_t m_depression_goal;
    int32_t m_x_pos;
    int32_t m_y_pos;
};

using Serf_p = uptr<Serf>;

#endif //FIEFDOM_SERF_HPP
