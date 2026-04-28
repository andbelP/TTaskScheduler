#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/weather.hpp"
#include "i_openweathermap_client.hpp"

class OpenWeatherMapClient : public IOpenWeatherMapClient {
    std::string api_key_;

public:

    std::expected<Weather, std::string> Get(Geoposition geoposition) const override;

};