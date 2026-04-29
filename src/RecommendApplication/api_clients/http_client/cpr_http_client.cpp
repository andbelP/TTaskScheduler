#include "cpr_http_client.hpp"

#include <cpr/cpr.h>

std::expected<nlohmann::json, std::string> CprHttpClient::Get(
    std::string url,
    std::vector<std::pair<std::string, std::string>> parameters) const {
    cpr::Parameters cpr_parameters;
    for (const auto& arg : parameters) {
        cpr_parameters.Add({arg.first, arg.second});
    }

    auto response = cpr::Get(cpr::Url{url}, cpr_parameters);

    if (response.status_code != 200) {
        return std::unexpected("HTTP request failed, status code != 200. " +
                               std::to_string(response.status_code));
    }

    try {
        return nlohmann::json::parse(response.text);
    } catch (const std::exception& e) {
        return std::unexpected("Failed to parse JSON response: " +
                               std::string(e.what()));
    }
}