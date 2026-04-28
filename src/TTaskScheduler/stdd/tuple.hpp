#pragma once

#include <cstddef>

#include "utils.hpp"

namespace stdd{
    
template<typename...> 
class tuple;

template<>
class tuple<>{
public:
    bool operator==(const tuple<>& other) const {
        return true;
    }
};

template<typename Head, typename... Tail>
class tuple<Head, Tail...> : public tuple<Tail...> {
    Head value;
public:

    bool operator==(const tuple<Head, Tail...>& other) const {
        return value == other.value && static_cast<const tuple<Tail...>&>(*this) == static_cast<const tuple<Tail...>&>(other);
    }

    template<typename THead, typename... TTail>
    tuple(THead&& head, TTail&&... tail) : value(stdd::forward<THead>(head)), tuple<Tail...>(stdd::forward<TTail>(tail)...){}

    template<size_t ind>
    auto& get()&{
        if constexpr(ind==0){
            return value;
        }
        else{
            return static_cast<tuple<Tail...>&>(*this).template get<ind-1>();
        }
    }

    template<size_t ind>
    auto&& get() && {
        if constexpr(ind==0){
            return stdd::move(value);
        }
        else{
            return static_cast<tuple<Tail...>&&>(*this).template get<ind-1>();
        }
    }

    template<size_t ind>
    const auto& get() const &{
        if constexpr(ind==0){
            return value;
        }
        else{
            return static_cast< const tuple<Tail...>&>(*this).template get<ind-1>();
        }
    }
};

template<size_t ind, typename Tuple>
decltype(auto) get(Tuple&& t){
    return stdd::forward<Tuple>(t).template get<ind>();
}



}
