# Automaticly generated by wsjcpp@v0.1.6
cmake_minimum_required(VERSION 3.0)

project(unit-tests C CXX)
add_definitions(-DWSJCPP_APP_VERSION="ut-v0.0.2")
add_definitions(-DWSJCPP_APP_NAME="unit-tests-wsjcpp-obj-tree")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${unit-tests_SOURCE_DIR})

set (WSJCPP_LIBRARIES "")
set (WSJCPP_INCLUDE_DIRS "")
set (WSJCPP_SOURCES "")

find_package(Threads REQUIRED)
list (APPEND WSJCPP_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# wsjcpp-core:v0.2.1
list (APPEND WSJCPP_INCLUDE_DIRS "../src.wsjcpp/wsjcpp_core/")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_core.h")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_unit_tests.cpp")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_unit_tests.h")
list (APPEND WSJCPP_SOURCES "../src.wsjcpp/wsjcpp_core/wsjcpp_unit_tests_main.cpp")

# wsjcpp-obj-tree:v0.0.2
list (APPEND WSJCPP_INCLUDE_DIRS "../src")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_obj_tree.h")
list (APPEND WSJCPP_SOURCES "../src/wsjcpp_obj_tree.cpp")

# unit-tests
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_add_node.h")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_add_node.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_write_tree.h")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_write_tree.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_find_nodes.h")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_find_nodes.cpp")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_read_tree.h")
list (APPEND WSJCPP_SOURCES "../unit-tests.wsjcpp/src/unit_test_read_tree.cpp")

include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.user-custom.txt)


include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable ("unit-tests" ${WSJCPP_SOURCES})

target_link_libraries("unit-tests" -lpthread ${WSJCPP_LIBRARIES} )

install(
    TARGETS
        "unit-tests"
    RUNTIME DESTINATION
        /usr/bin
)


