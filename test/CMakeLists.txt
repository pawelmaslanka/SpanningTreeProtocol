cmake_minimum_required(VERSION 3.0)

message(STATUS "Building Spanning Tree Protocol tests...")

include(unit_tests.cmake RESULT_VARIABLE TEST_CONF)

add_definitions(-DUNIT_TEST)

set(TEST_ROOT_DIR ${PROJECT_SOURCE_DIR}/test)

add_subdirectory(mock)
add_subdirectory(source)
