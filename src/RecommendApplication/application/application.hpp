#pragma once

#include <expected>
#include <string>

#include "api_clients/geo_by_ip_client/i_ip_geopos_client.hpp"
#include "api_clients/weather_client/i_openweathermap_client.hpp"
#include "api_clients/maps_client/i_overpass_client.hpp"
#include "api_clients/distance_client/i_distance_client.hpp"

class Application{

    int cnt_of_places_;
    const IIpGeoPosClient& two_ip_client_;
    const IOpenWeatherMapClient& openweathermap_client_;
    const IOverpassClient& overpass_client_;
    const IDistanceClient& distance_client_;

public:

    Application(int cnt_of_places, const IIpGeoPosClient& two_ip_client, const IOpenWeatherMapClient& openweathermap_client, const IOverpassClient& overpass_client, const IDistanceClient& distance_client) : cnt_of_places_(cnt_of_places), two_ip_client_(two_ip_client), openweathermap_client_(openweathermap_client), overpass_client_(overpass_client), distance_client_(distance_client) {}

    std::expected<void, std::string> Run();

};