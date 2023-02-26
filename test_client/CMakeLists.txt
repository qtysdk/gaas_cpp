cmake_minimum_required(VERSION 3.5)
project(gaas_console CXX)


add_executable(${PROJECT_NAME} console.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE usecase)
