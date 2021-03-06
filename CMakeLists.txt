cmake_minimum_required(VERSION 3.0)

project(wsjcpp-obj-tree C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${wsjcpp-obj-tree_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/examples")

list (APPEND WSJCPP_SOURCES "./src/wsjcpp_obj_tree.h")
list (APPEND WSJCPP_SOURCES "./src/wsjcpp_obj_tree.cpp")
list (APPEND WSJCPP_SOURCES "src/main.cpp")

# examples
list (APPEND WSJCPP_SOURCES "./src/examples/wsjcpp_obj_tree_node_building.h")
list (APPEND WSJCPP_SOURCES "./src/examples/wsjcpp_obj_tree_node_building.cpp")

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (wsjcpp-obj-tree ${WSJCPP_SOURCES})

target_link_libraries(wsjcpp-obj-tree ${WSJCPP_LIBRARIES})

install(
    TARGETS
        wsjcpp-obj-tree
    RUNTIME DESTINATION
        /usr/bin
)


