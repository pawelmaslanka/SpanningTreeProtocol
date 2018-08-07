cmake_minimum_required(VERSION 3.0)

message(STATUS "Including unit tests dependencies...")

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#find_library(GTEST_LIBRARY
#                NAMES gtest
#                      libgtest
#                      libgtest.a
#                PATHS "${GTEST_DIR}"
#                PATH_SUFFIXES lib)

find_library(GTEST gtest)
find_library(GTEST_MAIN gtest_main)

find_library(GMOCK gmock)
find_library(GMOCK_MAIN gmock_main)

# adds the gtest include directory
include_directories(${GTEST_INCLUDE_DIRS})

if(GMOCK AND GMOCK_MAIN AND GTEST AND GTEST_MAIN AND GTEST_FOUND)
    message(STATUS "GTest found: ${GTEST}")
    message(STATUS "GTest main found: ${GTEST_MAIN}")
    message(STATUS "GMock found: ${GMOCK}")
    message(STATUS "GMock main found: ${GMOCK_MAIN}")

    include_directories(
        ${GTEST_INCLUDE_DIRECTORY}
        ${GMOCK_INCLUDE_DIRECTORY}
    )
    enable_testing()
else()
    message(FATAL "Test libraries not found")
endif()

# Set global variables of sources and its directories

# It is done by RSTP_SOURCES
#set(RSTP_SRCDIR ${CMAKE_CURRENT_LIST_DIR}/../)
set(MOCK_DIR ${CMAKE_CURRENT_LIST_DIR}/mock)
