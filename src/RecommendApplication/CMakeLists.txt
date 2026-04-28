add_executable(RecommendApp main.cpp application/application.cpp)


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

