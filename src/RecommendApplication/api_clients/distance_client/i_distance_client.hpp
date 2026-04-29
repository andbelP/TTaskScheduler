#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/distance.hpp"

class IDistanceClient {
public:

    virtual std::expected<Distance, std::string> Get(Geoposition from, Geoposition to) const = 0;

    virtual ~IDistanceClient() = default;
};