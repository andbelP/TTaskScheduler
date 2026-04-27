#pragma once

#include <cstddef>

#include "tuple.hpp"
#include "utils.hpp"


namespace stdd{



template<size_t... Indexes>
struct index_sequence{};

template<size_t N, size_t... Indexes>
struct make_index_sequence_impl{
    using type = typename make_index_sequence_impl<N-1, N-1, Indexes...>::type;
};

template<size_t... Indexes>
struct make_index_sequence_impl<0, Indexes...>{
    using type = index_sequence<Indexes...>;
};

template<size_t N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;



template <typename Arg>
decltype(auto) Unpack(Arg&& arg) {
    if constexpr (is_future_v<std::remove_cvref_t<Arg>>) {
        return stdd::forward<Arg>(arg).get();
    } else {
        return stdd::forward<Arg>(arg);
    }
}


template<typename T>
struct tuple_size;

template<typename... Args, template<typename...> typename Tuple>
struct tuple_size<Tuple<Args...>> {
    static constexpr std::size_t value = sizeof...(Args);
};



template <typename F, typename Tuple, std::size_t... Indxs>
decltype(auto) apply_impl(F&& func, Tuple&& tuple,
                          index_sequence<Indxs...> indxs) {
    return stdd::forward<F>(func)(Unpack(stdd::forward<Tuple>(tuple).template get<Indxs>())...);
}

template <typename F, typename Tuple>
decltype(auto) ApplyAndUnpack(F&& func, Tuple&& tuple) {
    return apply_impl(stdd::forward<F>(func), stdd::forward<Tuple>(tuple),
                      make_index_sequence<tuple_size<std::remove_cvref_t<Tuple>>::value>());
}

}