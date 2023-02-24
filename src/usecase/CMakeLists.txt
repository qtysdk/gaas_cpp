file(GLOB usecase_src *.cc)

add_library(usecase ${usecase_src})
target_include_directories(usecase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(usecase PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(usecase PRIVATE repo model)
