#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/weather.hpp"
#include "i_overpass_client.hpp"
#include "api_clients/http_client/i_http_client.hpp"

class OverpassClient : public IOverpassClient {
    std::string api_key_;
    const IHttpClient& http_client_;
public:
    OverpassClient(const IHttpClient& http_client, const std::string& api_key = "dont need api key") : api_key_(api_key), http_client_(http_client) {}

    std::expected<std::vector<InterestingPlace>, std::string> Get(Geoposition geoposition, Weather weather, int cnt_of_places) const override;

};