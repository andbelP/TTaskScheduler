#pragma once

enum class WeatherType{
    kRain,
    kSnow,
    kSunny,
    kCloudy
};

struct Weather{
    double temperature;
    WeatherType weather_type;
};