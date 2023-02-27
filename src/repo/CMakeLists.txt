file(GLOB repo_src *.cc)

add_library(repo ${repo_src})
target_link_libraries(repo PRIVATE model)
add_dependencies(repo model)

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
target_link_libraries(repo PUBLIC mongo::bsoncxx_shared)
target_link_libraries(repo PUBLIC mongo::mongocxx_shared)