#pragma once

#include <expected>
#include <string>

#include "api_clients/two_ip_client/i_twoip_client.hpp"
#include "api_clients/openweathermap_client/i_openweathermap_client.hpp"
#include "api_clients/overpass_client/i_overpass_client.hpp"

class Application{

    int cnt_of_places_;
    const ITwoIpClient& two_ip_client_;
    const IOpenWeatherMapClient& openweathermap_client_;
    const IOverpassClient& overpass_client_;

public:

    Application(int cnt_of_places, const ITwoIpClient& two_ip_client, const IOpenWeatherMapClient& openweathermap_client, const IOverpassClient& overpass_client) : cnt_of_places_(cnt_of_places), two_ip_client_(two_ip_client), openweathermap_client_(openweathermap_client), overpass_client_(overpass_client) {}

    std::expected<void, std::string> Run();

};