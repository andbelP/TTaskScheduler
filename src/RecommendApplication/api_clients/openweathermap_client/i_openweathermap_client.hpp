#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/weather.hpp"

class IOpenWeatherMapClient {
public:

    virtual std::expected<Weather, std::string> Get(Geoposition geoposition) const = 0;

    virtual ~IOpenWeatherMapClient() = default;

};