#pragma once

enum class WeatherType{
    kWalkable,
    kNotWalkable
};

struct Weather{
    double temperature;
    WeatherType weather_type;
};