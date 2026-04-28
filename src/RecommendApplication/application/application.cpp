#include <expected>
#include <string>
#include <iostream>

#include "application.hpp"
#include "errors_handling/check_expected.hpp"

std::expected<void, std::string> Application::Run(){
    auto geoposition = two_ip_client_.Get();
    PROPAGATE_EXPECTED(geoposition);


    auto weather = openweathermap_client_.Get(geoposition.value());
    PROPAGATE_EXPECTED(weather);

    auto interesting_places = overpass_client_.Get(geoposition.value(), weather.value(), cnt_of_places_);
    PROPAGATE_EXPECTED(interesting_places);

    for (const auto& place : interesting_places.value()) {
        std::cout << place.name << "\n";
    }

    return {};
}