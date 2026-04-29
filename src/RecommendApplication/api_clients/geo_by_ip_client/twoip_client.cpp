#include <expected>

#include "twoip_client.hpp"
#include "errors_handling/check_expected.hpp"

#include <nlohmann/json.hpp>


std::expected<Geoposition, std::string> TwoIpClient::Get() const {
    try{
        auto result_expected = http_client_.Get("https://api.2ip.io", {{"token", api_key_}});
        PROPAGATE_EXPECTED(result_expected);

        auto& result = result_expected.value();

        double latitude = std::stod(result.at("lat").get<std::string>());
        double longitude = std::stod(result.at("lon").get<std::string>());

        return Geoposition(latitude, longitude);
    }
    catch(const std::exception& e){
        return std::unexpected("Unexpected JSON from 2IP: " + std::string(e.what()));
    }
}