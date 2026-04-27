cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Scheduler)

add_executable(scheduler bin/main.cpp)

target_include_directories(scheduler PRIVATE lib)

enable_testing()
add_subdirectory(tests)