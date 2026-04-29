#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "models/distance.hpp"

#include "api_clients/http_client/i_http_client.hpp"
#include "i_distance_client.hpp"

class OSRMClient : public IDistanceClient {
    std::string api_key_;
    const IHttpClient& http_client_;

public:

    OSRMClient(const IHttpClient& http_client, const std::string& api_key = "dont need apikey") : api_key_(api_key), http_client_(http_client) {}

    std::expected<Distance, std::string> Get(Geoposition from, Geoposition to) const override;

};