file(GLOB repo_src *.cc)

add_library(repo ${repo_src})
target_include_directories(repo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(repo PRIVATE model)