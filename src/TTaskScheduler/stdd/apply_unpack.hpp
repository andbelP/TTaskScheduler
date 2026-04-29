#pragma once

#include <cstddef>

#include "tuple.hpp"
#include "utils.hpp"
#include "invoke.hpp"
#include "metafunctions.hpp"


namespace stdd{




template <typename F, typename Tuple, typename... Args, size_t... Indxs>
decltype(auto) UnpackAndInvokeImpl(F&& func, Tuple&& tuple, std::index_sequence<Indxs...> indxs) {
    return stdd::invoke(stdd::forward<F>(func), (stdd::forward<Tuple>(tuple).template get<Indxs>().Unpack())...);
}




template <typename F, typename Tuple, typename... Args>
decltype(auto) InvokeAndUnpack(F&& func, Tuple&& tuple) {
    return UnpackAndInvokeImpl(stdd::forward<F>(func), stdd::forward<Tuple>(tuple), std::make_index_sequence<stdd::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

}