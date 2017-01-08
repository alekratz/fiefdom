#ifndef FIEFDOM_UTIL_HPP
#define FIEFDOM_UTIL_HPP

#include "types.hpp"
#include <spdlog/spdlog.h>

#define CENTER(l, s) (l - (s / 2))

struct Loggable {
    Loggable(cstref name) {
        m_logger = spdlog::get(name);
        if(m_logger == nullptr)
            m_logger = spdlog::stdout_logger_mt(name, true);
    }
    sptr<spdlog::logger> m_logger;
};

#endif //FIEFDOM_UTIL_HPP