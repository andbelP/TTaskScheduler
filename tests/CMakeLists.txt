include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    FIND_PACKAGE_ARGS NAMES GTest
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    scheduler-lib-tests
    test_scheduler.cpp
    test_any.cpp
    test_tuple.cpp
)

target_link_libraries(
    scheduler-lib-tests
    GTest::gtest_main
    GTest::gmock_main
)

target_include_directories(
    scheduler-lib-tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)


include(GoogleTest)

gtest_discover_tests(scheduler-lib-tests)
