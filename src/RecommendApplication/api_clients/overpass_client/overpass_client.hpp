#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/weather.hpp"
#include "i_overpass_client.hpp"

class OverpassClient : public IOverpassClient {
    std::string api_key_;
public:
    OverpassClient(const std::string& api_key) : api_key_(api_key) {}

    std::expected<std::vector<InterestingPlace>, std::string> Get(Geoposition geoposition, Weather weather, int cnt_of_places) const override;

};