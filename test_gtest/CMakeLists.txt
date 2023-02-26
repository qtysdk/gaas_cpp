cmake_minimum_required(VERSION 3.5)
project(gaas_cpp_gtest CXX)


file(GLOB web_src ../src/web/*.cc)

add_executable(${PROJECT_NAME} test_main.cc ${web_src})

# add dependencies
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Jsoncpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIRS})

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)
#
# and comment out the following lines


target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon Jsoncpp_lib usecase ${GTEST_BOTH_LIBRARIES})
