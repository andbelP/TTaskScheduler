#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"

class ITwoIpClient{
public:
    virtual std::expected<Geoposition, std::string> Get() const = 0;
    
    virtual ~ITwoIpClient() = default;
};