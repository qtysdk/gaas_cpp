file(GLOB usecase_src *.cc)

add_library(usecase ${usecase_src})
target_link_libraries(usecase PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(usecase PRIVATE repo model)
add_dependencies(usecase repo)