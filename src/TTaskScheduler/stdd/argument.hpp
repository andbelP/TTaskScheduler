#pragma once

#include "TTaskScheduler/TFuture.hpp"
#include "TTaskScheduler/stdd/utils.hpp"

namespace stdd{

template<typename T>
class Argument{
    using value_type = std::decay_t<T>;
    value_type value_;
public:
    template<typename TArg>
    Argument(TArg&& value) : value_(stdd::forward<TArg>(value)) {}

    decltype(auto) Unpack(){
        if constexpr(is_future_v<T>){
            return value_.get();
        }
        else{
            if constexpr(std::is_lvalue_reference_v<T>){
                return (value_);
            }
            else{
                return stdd::move(value_);
            }
        }
    }
};

}