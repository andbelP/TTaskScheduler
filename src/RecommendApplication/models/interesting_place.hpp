#pragma once

#include <string>

#include "geoposition.hpp"

struct InterestingPlace{
    Geoposition geoposition;
    std::string name;

    std::string website;
    std::string email;
    std::string info;
    std::string description;
};