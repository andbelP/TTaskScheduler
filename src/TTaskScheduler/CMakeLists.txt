add_library(TTaskScheduler INTERFACE)

target_compile_features(TTaskScheduler
    INTERFACE
)

target_include_directories(TTaskScheduler
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)