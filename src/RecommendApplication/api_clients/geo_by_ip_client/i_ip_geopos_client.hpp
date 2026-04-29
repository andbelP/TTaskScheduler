#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"

class IIpGeoPosClient{
public:
    virtual std::expected<Geoposition, std::string> Get() const = 0;
    
    virtual ~IIpGeoPosClient() = default;
};