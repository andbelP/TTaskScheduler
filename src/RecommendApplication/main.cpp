#include <iostream>

#include "application/application.hpp"
#include "api_clients/geo_by_ip_client/twoip_client.hpp"
#include "api_clients/weather_client/openweathermap_client.hpp"
#include "api_clients/maps_client/overpass_client.hpp"
#include "api_clients/distance_client/OSRM_client.hpp"
#include "api_clients/http_client/cpr_http_client.hpp"


int main(int argc, char** argv){
    int N = 10;
    if(argc == 2){
        N = std::stoi(std::string(argv[1]));
    }

    CprHttpClient http_client;

    TwoIpClient two_ip_client("wvov15ahnquoi9lv", http_client);

    OpenWeatherMapClient openweathermap_client("e7ecc1aaf49378d4b45e6b3f4c2f8c92", http_client);

    OverpassClient overpass_client(http_client);

    OSRMClient distance_client(http_client);

    Application application(N, two_ip_client, openweathermap_client, overpass_client, distance_client);

    auto result = application.Run();
    if(!result){
        std::cerr << "Error: " << result.error() << "\n";
        return 1;
    }
}