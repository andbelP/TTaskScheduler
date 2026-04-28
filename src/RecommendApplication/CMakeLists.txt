add_executable(RecommendApp main.cpp application/application.cpp)

target_link_libraries(RecommendApp PRIVATE TTaskScheduler)

