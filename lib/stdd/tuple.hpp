#pragma once

namespace stdd{
    
template<typename...> 
class Tuple;

template<>
class Tuple<>{

};

template<typename Head, typename... Tail>
class Tuple<Head, Tail...> : public Tuple<Tail...> {
    Head value;
public:
    Tuple(const Head& head, const Tail&... tail) : value(head), Tuple<Tail...>(tail...){}

    template<unsigned ind>
    auto& get(){
        if constexpr(ind==0){
            return value;
        }
        else{
            return static_cast<Tuple<Tail...>&>(*this).template get<ind-1>();
        }
    }

    template<unsigned ind>
    const auto& get() const{
        if constexpr(ind==0){
            return value;
        }
        else{
            return static_cast<const Tuple<Tail...>&>(*this).template get<ind-1>();
        }
    }
};

}

