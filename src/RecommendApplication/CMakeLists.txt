add_executable(RecommendApp main.cpp 
    application/application.cpp 
    api_clients/http_client/cpr_http_client.cpp
    api_clients/weather_client/openweathermap_client.cpp
    api_clients/geo_by_ip_client/twoip_client.cpp
    api_clients/maps_client/overpass_client.cpp
    api_clients/distance_client/OSRM_client.cpp
)


include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG master)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git
                         GIT_TAG master)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(RecommendApp 
    PRIVATE 
    TTaskScheduler
    cpr::cpr
    nlohmann_json::nlohmann_json
)

target_include_directories(RecommendApp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

