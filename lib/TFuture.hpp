#pragma once

#include "ITask.hpp"

template <typename T>
class TFuture {
    std::shared_ptr<ITask> task_;

   public:
    TFuture(std::shared_ptr<ITask> task) : task_(task) {}

    T get() const;
};

template <typename T>
T TFuture<T>::get() const {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return std::any_cast<T>(task_->GetResultRef());
    } else {
        return std::any_cast<T>(task_->GetResult());
    }
}


template <typename T>
class TFuture;

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