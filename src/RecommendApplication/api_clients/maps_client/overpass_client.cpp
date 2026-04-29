#include "overpass_client.hpp"
#include "errors_handling/check_expected.hpp"
#include <nlohmann/json.hpp>

namespace{

std::string GetOverpassQuery(Geoposition geoposition, Weather weather, int cnt_of_places){

    std::string geo = "(around:2000," + std::to_string(geoposition.latitude) + "," + std::to_string(geoposition.longitude) + ")";

    if (weather.weather_type == WeatherType::kWalkable) {
        return std::string("[out:json][timeout:25];(")+"nwr" + geo + "[tourism];"
               "nwr" + geo + "[historic];"
               "nwr" + geo + "[amenity~\"^(cafe|restaurant|bar|pub|theatre|cinema|library)$\"];"
               "nwr" + geo + "[leisure~\"^(park|garden)$\"];"
               ");"
               "out center " + std::to_string(cnt_of_places) + ";";
    }

    return std::string("[out:json][timeout:25];(") +
       "nwr" + geo + "[amenity~\"^(cafe|restaurant|bar|pub|theatre|cinema|library)$\"];"
       "nwr" + geo + "[tourism~\"^(museum|gallery)$\"];"
       "nwr" + geo + "[shop];"
       ");"
       "out center " + std::to_string(cnt_of_places) + ";";

}

}

std::expected<std::vector<InterestingPlace>, std::string> OverpassClient::Get(Geoposition geoposition, Weather weather, int cnt_of_places) const {
    try{
        auto result_expected = http_client_.Get("https://overpass-api.de/api/interpreter", {{"data", GetOverpassQuery(geoposition, weather, cnt_of_places)}});
        PROPAGATE_EXPECTED(result_expected);

        auto& result = result_expected.value();

        std::vector<InterestingPlace> interesting_places;

        for (const auto& element : result.at("elements")) {
            std::string name = element.at("tags").value("name", "Unnamed Place");

            std::string website = element.at("tags").value("website", "No website");
            std::string email = element.at("tags").value("email", "No email");
            std::string info = element.at("tags").value("information", "No information");
            std::string description = element.at("tags").value("description", element.at("tags").value("inscription", "No description"));



            double latitude = element.at("lat").get<double>();
            double longitude = element.at("lon").get<double>();

            interesting_places.push_back(InterestingPlace{Geoposition{latitude, longitude}, name, website, email, info, description});
        }

        return interesting_places;
    }
    catch(const std::exception& e){
        return std::unexpected("Unexpected JSON from Overpass API: " + std::string(e.what()));
    }
}