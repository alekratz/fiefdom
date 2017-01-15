#ifndef FIEFDOM_UTIL_HPP
#define FIEFDOM_UTIL_HPP

#include "types.hpp"
#include <spdlog/spdlog.h>

#define CENTER(a, b) (((a) - (b)) / 2)

struct Loggable {
    Loggable(cstref name) {
        m_logger = spdlog::get(name);
        if(m_logger == nullptr)
            m_logger = spdlog::stdout_color_mt(name);
    }
    sptr<spdlog::logger> m_logger;
};

#endif //FIEFDOM_UTIL_HPP
