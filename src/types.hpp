#ifndef FIEFDOM_TYPES_HPP
#define FIEFDOM_TYPES_HPP

#include <string>
#include <vector>
#include <memory>

using str = std::string;
using cstref = const str&;

template<typename T>
using vec = std::vector<T>;

template<typename T>
using uptr = std::unique_ptr<T>;
template<typename T>
using sptr = std::shared_ptr<T>;
template<typename T>
using wptr = std::weak_ptr<T>;

/**
 * Game types
 */
template<typename T>
using Entity_ptr = uptr<T>;

class Entity {
public:
    virtual void draw() = 0;
    virtual void update() = 0;
};
using Entity_p = Entity_ptr<Entity>;

#endif //FIEFDOM_TYPES_HPP
