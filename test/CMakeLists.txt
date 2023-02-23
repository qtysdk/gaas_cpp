cmake_minimum_required(VERSION 3.5)
project(gaas_cpp_test CXX)

add_executable(${PROJECT_NAME} test_main.cc ../NothingCtrl.cc ../CreateGameCtrl.cc ../CreateGameUseCase.cc)

# add dependencies
include(../cmake/CPM.cmake)
CPMAddPackage("gh:nlohmann/json@3.10.5")

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)
#
# and comment out the following lines
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon nlohmann_json::nlohmann_json)

ParseAndAddDrogonTests(${PROJECT_NAME})
