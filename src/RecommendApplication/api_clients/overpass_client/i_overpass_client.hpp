#pragma once

#include <string>
#include <expected>
#include <vector>

#include "models/geoposition.hpp"
#include "models/weather.hpp"
#include "models/interesting_place.hpp"

class IOverpassClient {
public:
    virtual std::expected<std::vector<InterestingPlace>, std::string> Get(Geoposition geoposition, Weather weather, int cnt_of_places) const = 0;
    virtual ~IOverpassClient() = default;
};