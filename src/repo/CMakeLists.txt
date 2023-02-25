file(GLOB repo_src *.cc)

add_library(repo ${repo_src})
target_link_libraries(repo PRIVATE model)
add_dependencies(repo model)