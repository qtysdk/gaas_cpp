file(GLOB model_src *.cc)

add_library(model ${model_src})
target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})