file(GLOB usecase_src *.cc)

# add dependencies
include(../../cmake/CPM.cmake)
CPMAddPackage("gh:nlohmann/json@3.10.5")


add_library(usecase ${usecase_src})
target_link_libraries(usecase PRIVATE nlohmann_json::nlohmann_json repo model)
add_dependencies(usecase repo)