#pragma once

#include <string>
#include <expected>

#include <nlohmann/json.hpp>

class IHttpClient {
   public:
    virtual std::expected<nlohmann::json, std::string> Get(
        std::string url,
        std::vector<std::pair<std::string, std::string>> parameters) const = 0;

    virtual ~IHttpClient() = default;
};