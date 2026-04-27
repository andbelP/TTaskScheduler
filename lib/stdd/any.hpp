#pragma once

#include <memory>
#include "utils.hpp"

namespace stdd{

class AnyBase{
public:

    virtual ~AnyBase() = default;

    virtual std::unique_ptr<AnyBase> Clone() const = 0;

    virtual const std::type_info* GetTypeInfo() const = 0;

};

class any;

template<typename T>
class AnyHolder : public AnyBase{
    T value_;
    const std::type_info* type_info_;

    template<typename Type, typename Any>
    friend Type any_cast(Any&& val);

public:

    template<typename ValType>
    AnyHolder(ValType&& value) : value_(std::forward<ValType>(value)), type_info_(&typeid(std::decay_t<ValType>)){}

    std::unique_ptr<AnyBase> Clone() const override {
        if constexpr (std::is_copy_constructible_v<T>) {
            return std::make_unique<AnyHolder<T>>(value_);
        } else {
            throw std::runtime_error("Can't copy move_only type");
        }
    }

    const std::type_info* GetTypeInfo() const override{
        return type_info_;
    }

};

class any{
    std::unique_ptr<AnyBase> ptr_;

    
    template<typename T, typename Any>
    friend T any_cast(Any&& val);

public:

    any() = default;

    template<typename T>
    requires (!std::is_same_v<std::remove_cvref_t<T>, any>)
    any(T&& obj) : ptr_(std::make_unique<AnyHolder<std::decay_t<T>>>(stdd::forward<T>(obj))){}

    any(const any& other) : ptr_(other.ptr_ ? other.ptr_->Clone() : nullptr) {}

    any(any&& other) : ptr_(stdd::move(other.ptr_)){}

    any& operator=(const any& other){
        if(this==&other){
            return *this;
        }
        ptr_ = other.ptr_ ? other.ptr_->Clone() : nullptr;
        return *this;
    }

    any& operator=(any&& other) {
        ptr_ = stdd::move(other.ptr_);
        return *this;
    }

};


template<typename T, typename Any>
T any_cast(Any&& val){

    if(!val.ptr_ || typeid(T) != *(val.ptr_->GetTypeInfo())){
        throw std::bad_cast{};
    }

    auto* any_holder = static_cast<AnyHolder<std::decay_t<T>>*>(val.ptr_.get());

    if constexpr (std::is_lvalue_reference_v<Any&&>) {
        return static_cast<T>(any_holder->value_);
    } else {
        return static_cast<T>(stdd::move(any_holder->value_));
    }

}

}