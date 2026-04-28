#pragma once

#include <string>

#include "Geoposition.hpp"

struct InterestingPlace{
    Geoposition geoposition;
    std::string name;
    std::string description;
};