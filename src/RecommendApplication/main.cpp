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

        if(N <= 0){
            std::cerr << "Invalid number of places: " << N << "\n";
            return 1;
        }

        auto raw_2ip_api_key = std::getenv("IP2_API_KEY");

        if (raw_2ip_api_key == nullptr || std::string(raw_2ip_api_key).empty()) {
            std::cerr << "Cannot read 2IP api key";
            return 1;
        }

        auto raw_openweathermap_api_key = std::getenv("OPENWEATHERMAP_API_KEY");

        if (raw_openweathermap_api_key == nullptr || std::string(raw_openweathermap_api_key).empty()) {
            std::cerr << "Cannot read OpenWeatherMap api key";
            return 1;
        }

        CprHttpClient http_client;

        TwoIpClient two_ip_client(raw_2ip_api_key, http_client);

        OpenWeatherMapClient openweathermap_client(raw_openweathermap_api_key, http_client);

        OverpassClient overpass_client(http_client);

        OSRMClient distance_client(http_client);

        Application application(N, two_ip_client, openweathermap_client, overpass_client, distance_client);

        auto result = application.Run();
        if(!result){
            std::cerr << "Error: " << result.error() << "\n";
            return 1;
        }
    }