#pragma once

#include <cstddef>

#include "tuple.hpp"
#include "utils.hpp"
#include "invoke.hpp"
#include "metafunctions.hpp"


namespace stdd{

template <bool Move,typename Arg>
decltype(auto) Unpack(Arg&& arg) {
    if constexpr (is_future_v<std::remove_cvref_t<Arg>>) {
        return arg.get();
    } else if constexpr (Move) {
        return stdd::move(arg);
    } else {
        return arg;
    }
}

template<std::size_t Mask, typename Func, typename Tuple, std::size_t... Indxs>
auto InvokeWithMask(Func&& func, Tuple&& tuple, index_sequence<Indxs...> indxs) -> decltype(stdd::invoke(stdd::forward<Func>(func), Unpack<(Mask & (1 << Indxs)) != 0>(stdd::forward<Tuple>(tuple).template get<Indxs>())...)) {
    return stdd::invoke(stdd::forward<Func>(func), Unpack<(Mask & (1 << Indxs)) != 0>(stdd::forward<Tuple>(tuple).template get<Indxs>())...);
}

template<std::size_t Mask, typename Func, typename Tuple, std::size_t... Indxs>
concept CanExecuteWithMask = requires (Func&& func, Tuple&& tuple, stdd::index_sequence<Indxs...> indxs) {
    InvokeWithMask<Mask>(stdd::forward<Func>(func), stdd::forward<Tuple>(tuple), indxs);
};

template<std::size_t Mask, typename Func, typename Tuple, std::size_t... Indxs>
decltype(auto) InvokeWithMaskRec(Func&& func, Tuple&& tuple, stdd::index_sequence<Indxs...> indxs) {
    if constexpr(CanExecuteWithMask<Mask, Func, Tuple, Indxs...>){
        return InvokeWithMask<Mask>(stdd::forward<Func>(func), stdd::forward<Tuple>(tuple), indxs);
    }
    else{
        return InvokeWithMaskRec<Mask - 1>(stdd::forward<Func>(func), stdd::forward<Tuple>(tuple), indxs);
    }
}


template<typename Func, typename Tuple, typename Enable = void>
decltype(auto) InvokeWithOptionalMoving(Func&& func, Tuple&& tuple) {
    return InvokeWithMaskRec<(1 << tuple_size_v<Tuple>) - 1>(stdd::forward<Func>(func), stdd::forward<Tuple>(tuple), stdd::make_index_sequence<tuple_size_v<Tuple>>{});
}


template <typename F, typename Tuple>
decltype(auto) InvokeAndUnpack(F&& func, Tuple&& tuple) {
    return stdd::InvokeWithOptionalMoving(stdd::forward<F>(func), stdd::forward<Tuple>(tuple));
}

}