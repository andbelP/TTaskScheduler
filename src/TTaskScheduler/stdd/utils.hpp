#pragma once

#include "type_traits"

namespace stdd{

template<typename T>
std::remove_reference_t<T>&& move(T&& value){
    return static_cast<std::remove_reference_t<T>&&>(value);  
}

template<typename T>
T&& forward(std::remove_reference_t<T>& value){
    return static_cast<T&&>(value);  
}


template<typename T>
T&& forward(std::remove_reference_t<T>&& value){
    return static_cast<T&&>(value);  
}




}