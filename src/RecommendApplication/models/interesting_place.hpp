#pragma once

#include <string>

#include "geoposition.hpp"

struct InterestingPlace{
    Geoposition geoposition;
    std::string name;
    std::string description;
};