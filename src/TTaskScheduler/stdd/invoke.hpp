#pragma once

#include "utils.hpp"

namespace stdd {

template <typename T, typename Class, typename Obj, typename... Args>
auto invoke(T Class::* method, Obj&& obj, Args&&... args) {
    return (stdd::forward<Obj>(obj).*method)(stdd::forward<Args>(args)...);
}

template <typename T, typename Class, typename Obj, typename... Args>
auto invoke(T Class::* method, Obj* obj, Args&&... args) {
    return (obj->*method)(stdd::forward<Args>(args)...);
}

template <typename F, typename... Args>
auto invoke(F&& f, Args&&... args) {
    return stdd::forward<F>(f)(stdd::forward<Args>(args)...);
}

}  // namespace stdd