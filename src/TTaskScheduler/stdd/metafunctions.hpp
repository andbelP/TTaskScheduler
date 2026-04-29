#pragma once

#include <type_traits>

namespace stdd {

template <typename T>
struct first_argument;

template <typename R, typename Arg>
struct first_argument<R (*)(Arg)> {
    using type = Arg;
};

template <typename R, typename Arg>
struct first_argument<R (&)(Arg)> {
    using type = Arg;
};

template <typename R, typename Arg>
struct first_argument<R (*)(Arg) noexcept> {
    using type = Arg;
};

template <typename R, typename Arg>
struct first_argument<R (&)(Arg) noexcept> {
    using type = Arg;
};

template <typename C, typename R, typename Arg>
struct first_argument<R (C::*)(Arg)> {
    using type = Arg;
};

template <typename C, typename R, typename Arg>
struct first_argument<R (C::*)(Arg) const> {
    using type = Arg;
};

template <typename C, typename R, typename Arg>
struct first_argument<R (C::*)(Arg) noexcept> {
    using type = Arg;
};

template <typename C, typename R, typename Arg>
struct first_argument<R (C::*)(Arg) const noexcept> {
    using type = Arg;
};

template <typename Func, typename = void>
struct first_argument_callable {
    using type = typename first_argument<std::remove_reference_t<Func>>::type;
};

template <typename Func>
struct first_argument_callable<Func, std::void_t<decltype(&std::remove_reference_t<Func>::operator())>> {
    using type = typename first_argument<decltype(&std::remove_reference_t<Func>::operator())>::type;
};

template <typename Func>
using first_func_argument_t = typename first_argument_callable<Func>::type;


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


template<typename T>
struct tuple_size;

template<typename... Args, template<typename...> typename Tuple>
struct tuple_size<Tuple<Args...>> {
    static constexpr std::size_t value = sizeof...(Args);
};

template<typename Tuple>
constexpr std::size_t tuple_size_v = tuple_size<std::remove_cvref_t<Tuple>>::value;


}  // namespace stdd
