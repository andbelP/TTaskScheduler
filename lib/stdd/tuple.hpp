#pragma once

#include <cstddef>

namespace stdd{
    
template<typename...> 
class tuple;

template<>
class tuple<>{

};

template<typename Head, typename... Tail>
class tuple<Head, Tail...> : public tuple<Tail...> {
    Head value;
public:
    tuple(const Head& head, const Tail&... tail) : value(head), tuple<Tail...>(tail...){}

    template<size_t ind>
    auto& get(){
        if constexpr(ind==0){
            return value;
        }
        else{
            return static_cast<tuple<Tail...>&>(*this).template get<ind-1>();
        }
    }

    template<size_t ind>
    const auto& get() const{
        if constexpr(ind==0){
            return value;
        }
        else{
            return static_cast<const tuple<Tail...>&>(*this).template get<ind-1>();
        }
    }
};

template<typename... Args, size_t ind>
auto& get(tuple<Args...> t){
    return t.template get<ind>();
}

template<typename... Args, size_t ind>
const auto& get(const tuple<Args...> t){
    return t.template get<ind>();
}

}

