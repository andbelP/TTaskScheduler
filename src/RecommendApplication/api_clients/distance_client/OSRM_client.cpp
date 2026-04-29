#include "OSRM_client.hpp"

#include <expected>
#include "errors_handling/check_expected.hpp"
#include <nlohmann/json.hpp>


std::expected<Distance, std::string> OSRMClient::Get(Geoposition from, Geoposition to) const {
    try{
        std::string coordinates = std::to_string(from.longitude) + "," + std::to_string(from.latitude) + ";" + std::to_string(to.longitude) + "," + std::to_string(to.latitude);
        auto result_expected = http_client_.Get("http://router.project-osrm.org/route/v1/walking/" + coordinates, {{"overview", "false"}});
        PROPAGATE_EXPECTED(result_expected);

        auto& result = result_expected.value();

        double distance = result.at("routes").at(0).at("distance").get<double>();

        return Distance{distance};
    }
    catch(const std::exception& e){
        return std::unexpected("Unexpected JSON from OSRM API: " + std::string(e.what()));
    }
}