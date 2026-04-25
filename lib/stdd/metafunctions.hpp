#pragma once

namespace stdd {

template <typename T>
struct is_reference {
    static constexpr bool value = false;
};

template <typename T>
struct is_reference<T&> {
    static constexpr bool value = true;
};

template <typename T>
struct is_reference<T&&> {
    static constexpr bool value = true;
};

template <typename T>
constexpr bool is_reference_v = is_reference<T>::value;

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_volatile {
    using type = T;
};

template <typename T>
struct remove_volatile<volatile T> {
    using type = T;
};

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T>
using remove_cv_t = remove_const_t<remove_volatile_t<T>>;

template <typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

template <typename T>
struct array_decay {
    using type = T;
};

template <typename T>
struct array_decay<T[]> {
    using type = T*;
};

template <typename T, size_t N>
struct array_decay<T[N]> {
    using type = T*;
};

template <typename T>
using array_decay_t = typename array_decay<T>::type;

template <typename T>
struct function_decay {
    using type = T;
};

template <typename Ret, typename... Args>
struct function_decay<Ret(Args...)> {
    using type = Ret (*)(Args...);
};

template <typename Ret, typename... Args>
struct function_decay<Ret(Args...) noexcept> {
    using type = Ret (*)(Args...) noexcept;
};

template <typename T>
using function_decay_t = typename function_decay<T>::type;

template <typename T>
using decay_t =
    remove_cv_t<function_decay_t<array_decay_t<remove_reference_t<T>>>>;

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

}  // namespace stdd
