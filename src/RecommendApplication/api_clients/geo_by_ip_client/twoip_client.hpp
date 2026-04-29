#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "i_ip_geopos_client.hpp"
#include "api_clients/http_client/i_http_client.hpp"

class TwoIpClient : public IIpGeoPosClient {

    std::string api_key_;
    const IHttpClient& http_client_;

public:

    TwoIpClient(const std::string& api_key, const IHttpClient& http_client) : api_key_(api_key), http_client_(http_client) {}

    std::expected<Geoposition, std::string> Get() const override;

};