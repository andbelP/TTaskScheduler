#pragma once

#include <string>
#include <expected>

#include "models/geoposition.hpp"
#include "i_twoip_client.hpp"

class TwoIpClient : public ITwoIpClient {

    std::string api_key_;

public:

    TwoIpClient(const std::string& api_key) : api_key_(api_key) {}

    std::expected<Geoposition, std::string> Get() const override;

};