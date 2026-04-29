#pragma once

#include <expected>
#include <string>

#include "i_http_client.hpp"

class CprHttpClient : public IHttpClient {
   public:
    std::expected<nlohmann::json, std::string> Get(
        std::string url,
        std::vector<std::pair<std::string, std::string>> parameters)
        const override;
};