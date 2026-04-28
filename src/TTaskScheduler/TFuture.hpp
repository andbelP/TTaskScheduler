#pragma once

#include "ITask.hpp"

template <typename T>
class TFuture {
    std::shared_ptr<ITask> task_;

   public:
    TFuture(std::shared_ptr<ITask> task) : task_(task) {}

    decltype(auto) get();
};

template <typename T>
decltype(auto) TFuture<T>::get() {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return stdd::any_cast<T>(task_->GetResultRef());
    } else if constexpr (std::is_rvalue_reference_v<T>){
        return stdd::any_cast<T>(task_->GetResultRValueRef());
    }else {
        return stdd::any_cast<T>(task_->GetResult());
    }
}

template <typename T>
struct is_future {
    static constexpr bool value = false;
};

template <typename T>
struct is_future<TFuture<T>> {
    static constexpr bool value = true;
};

template <typename T>
constexpr bool is_future_v = is_future<T>::value;