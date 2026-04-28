#pragma once

#include "metafunctions.hpp"

namespace stdd{

template<typename T>
remove_reference_t<T>&& move(T&& value){
    return static_cast<remove_reference_t<T>&&>(value);  
}

template<typename T>
T&& forward(stdd::remove_reference_t<T>& value){
    return static_cast<T&&>(value);  
}


template<typename T>
T&& forward(stdd::remove_reference_t<T>&& value){
    return static_cast<T&&>(value);  
}




}