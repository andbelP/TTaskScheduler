#include <string>
#include <algorithm>
#include <vector>

#include "openweathermap_client.hpp"
#include "errors_handling/check_expected.hpp"

#include <nlohmann/json.hpp>

std::expected<Weather, std::string> OpenWeatherMapClient::Get(Geoposition geoposition) const {
    try{
        auto result_expected = http_client_.Get("https://api.openweathermap.org/data/2.5/weather", {{"lat", std::to_string(geoposition.latitude)}, {"lon", std::to_string(geoposition.longitude)}, {"appid", api_key_}, {"units", "metric"}});
        PROPAGATE_EXPECTED(result_expected);

        auto& result = result_expected.value();

        auto weather_type_str = result.at("weather").at(0).at("main").get<std::string>();
        double temperature = result.at("main").at("temp").get<double>();

        std::vector<std::string> walkable_types = {"Clear", "Snow", "Clouds"};

        if(std::ranges::find(walkable_types, weather_type_str) != walkable_types.end()){
            return Weather(temperature, WeatherType::kWalkable);
        }
        return Weather(temperature, WeatherType::kNotWalkable);
    }
    catch(const std::exception& e){
        return std::unexpected("Unexpected JSON from OpenWeatherMap: " + std::string(e.what()));
    }

}