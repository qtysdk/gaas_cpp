file(GLOB usecase_src *.cc)

# add dependencies
find_package(Jsoncpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIRS})

add_library(usecase ${usecase_src})
target_link_libraries(usecase PRIVATE Jsoncpp_lib repo model)
add_dependencies(usecase repo)