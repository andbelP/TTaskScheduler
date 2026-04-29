#include "application.hpp"

#include <expected>
#include <iostream>
#include <string>

#include "TTaskScheduler/TTaskScheduler.hpp"
#include "errors_handling/check_expected.hpp"

std::expected<void, std::string> Application::Run() {
    TTaskScheduler scheduler;

    auto geoposition_task =
        scheduler.add([this]() -> std::expected<Geoposition, std::string> {
            auto geoposition = two_ip_client_.Get();
            PROPAGATE_EXPECTED(geoposition);

            return geoposition.value();
        });

    auto weather_task = geoposition_task.apply(
        [this](std::expected<Geoposition, std::string>& geoposition_expected)
            -> std::expected<Weather, std::string> {
            PROPAGATE_EXPECTED(geoposition_expected);

            auto weather = openweathermap_client_.Get(geoposition_expected.value());
            PROPAGATE_EXPECTED(weather);

            return weather.value();
        });

    auto interesting_places_task = scheduler.add(
        [this](std::expected<Weather, std::string>& weather_expected,
               std::expected<Geoposition, std::string>& geoposition_expected,
               int cnt_of_places_)
            -> std::expected<std::vector<InterestingPlace>, std::string> {
            PROPAGATE_EXPECTED(weather_expected);
            PROPAGATE_EXPECTED(geoposition_expected);

            auto& weather = weather_expected.value();
            auto& geoposition = geoposition_expected.value();

            auto interesting_places =
                overpass_client_.Get(geoposition,
                                     weather, cnt_of_places_);
            PROPAGATE_EXPECTED(interesting_places);

            return interesting_places.value();
        },
        weather_task.getFutureResult<std::expected<Weather, std::string>&>(),
        geoposition_task.getFutureResult<std::expected<Geoposition, std::string>&>(),
        cnt_of_places_);


    auto print_task = scheduler.add(
        [this](std::expected<std::vector<InterestingPlace>, std::string>& interesting_places_expected,
               std::expected<Geoposition, std::string>& geoposition_expected)
            -> std::expected<void, std::string> {

            PROPAGATE_EXPECTED(interesting_places_expected);
            PROPAGATE_EXPECTED(geoposition_expected);

            auto& interesting_places = interesting_places_expected.value();
            auto& geoposition = geoposition_expected.value();

            for (const auto& place : interesting_places) {

                auto distance = distance_client_.Get(geoposition,place.geoposition); 
                PROPAGATE_EXPECTED(distance);

                std::cout << "\n-----------------------------------------\n";
                std::cout << "Name: " << place.name << "\n";
                std::cout << "Website: " << place.website << "\n";
                std::cout << "Email: " << place.email << "\n";
                std::cout << "Info: " << place.info << "\n";
                std::cout << "Description: " << place.description << "\n";
                std::cout << "Location: (" << place.geoposition.latitude << ", " << place.geoposition.longitude << ")\n"; 
                std::cout << "Distance: " << distance.value().distance << " meters\n"; 
                std::cout << "-----------------------------------------\n";
            }

            return {};

        }, interesting_places_task.getFutureResult<std::expected<std::vector<InterestingPlace>, std::string>&>(), geoposition_task.getFutureResult<std::expected<Geoposition, std::string>&>());


    scheduler.executeAll();

    return {};
}