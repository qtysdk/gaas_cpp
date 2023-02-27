cmake_minimum_required(VERSION 3.5)
project(gaas_cpp_gtest CXX)

file(GLOB web_src ../src/web/*.cc)
add_executable(${PROJECT_NAME} test_main.cc test_container.cc ${web_src})


# add dependencies
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Jsoncpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIRS})

find_package(Python3 REQUIRED COMPONENTS Development)
include_directories(${Python3_INCLUDE_DIRS})

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE mongo::bsoncxx_shared)
target_link_libraries(${PROJECT_NAME} PRIVATE mongo::mongocxx_shared)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Drogon::Drogon
        Jsoncpp_lib
        usecase
        ${GTEST_BOTH_LIBRARIES}
        ${Python3_LIBRARIES})

