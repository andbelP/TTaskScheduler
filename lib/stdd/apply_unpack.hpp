#pragma once

#include "tuple.hpp"
#include <cstddef>
#include <utility>


namespace stdd{

template <typename Arg>
decltype(auto) Unpack(Arg&& arg) {
    if constexpr (is_future_v<std::remove_cvref_t<Arg>>) {
        return std::forward<Arg>(arg).get();
    } else {
        return std::forward<Arg>(arg);
    }
}



template <typename F, typename... Args, std::size_t... Indxs>
decltype(auto) apply_impl(F&& func, stdd::Tuple<Args...>& tuple,
                          std::index_sequence<Indxs...> indxs) {
    return stdd::forward<F>(func)(Unpack(tuple.template get<Indxs>())...);
}

template <typename F, typename... Args>
decltype(auto) ApplyAndUnpack(F&& func, stdd::Tuple<Args...>& tuple) {
    return apply_impl(stdd::forward<F>(func), tuple,
                      std::make_index_sequence<sizeof...(Args)>());
}

}