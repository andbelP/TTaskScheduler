#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/weather.hpp"
#include "i_openweathermap_client.hpp"
#include "api_clients/http_client/i_http_client.hpp"

class OpenWeatherMapClient : public IOpenWeatherMapClient {
    std::string api_key_;
    const IHttpClient& http_client_;

public:

    OpenWeatherMapClient(const std::string& api_key, const IHttpClient& http_client) : api_key_(api_key), http_client_(http_client) {}

    std::expected<Weather, std::string> Get(Geoposition geoposition) const override;

};