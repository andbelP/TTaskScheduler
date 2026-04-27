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

    template<typename Type>
    friend Type any_cast(const any& val);

public:

    template<typename ValType>
    AnyHolder(ValType&& value) : value_(std::forward<ValType>(value)), type_info_(&typeid(std::decay_t<ValType>)){}

    std::unique_ptr<AnyBase> Clone() const override {
        return std::make_unique<AnyHolder<T>>(value_);
    }

    const std::type_info* GetTypeInfo() const override{
        return type_info_;
    }

};

class any{
    std::unique_ptr<AnyBase> ptr_;

    template<typename T>
    friend T any_cast(const any& val);

public:

    any() = default;

    template<typename T>
    requires (!std::is_same_v<std::remove_cvref_t<T>, any>)
    any(T&& obj) : ptr_(std::make_unique<AnyHolder<std::decay_t<T>>>(stdd::forward<T>(obj))){}\

    any(const any& other) : ptr_(other.ptr_->Clone()) {}

    any(any&& other) : ptr_(stdd::move(other.ptr_)){}

    any& operator=(const any& other){
        if(this==&other){
            return *this;
        }
        ptr_ = other.ptr_->Clone();
        return *this;
    }

    any& operator=(any&& other) {
        ptr_ = stdd::move(other.ptr_);
        return *this;
    }

};


template<typename T>
T any_cast(const any& val){

    if(!val.ptr_ || typeid(T) != *(val.ptr_->GetTypeInfo())){
        throw std::bad_cast{};
    }

    auto* any_holder = static_cast<AnyHolder<std::decay_t<T>>*>(val.ptr_.get());

    return std::forward<T>(any_holder->value_);

}

}