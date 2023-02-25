cmake_minimum_required(VERSION 3.5)
project(gaas_console CXX)


add_executable(${PROJECT_NAME} console.cc)

# add dependencies
include(../cmake/CPM.cmake)
CPMAddPackage("gh:nlohmann/json@3.10.5")

target_link_libraries(${PROJECT_NAME} PRIVATE usecase nlohmann_json::nlohmann_json)
