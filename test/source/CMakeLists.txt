cmake_minimum_required(VERSION 3.0)

message(STATUS "Unit testing of SpanningTree main directory...")
# Configure test environment
if(NOT TEST_CONF)
    include(${CMAKE_CURRENT_LIST_DIR}/../unit_tests.cmake RESULT_VARIABLE TEST_CONF)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

include_directories(.)
include_directories(${TEST_ROOT_DIR})
include_directories(${PROJECT_INCLUDE})

set(PTI_SM_UT port_timers_sm_ut)
set(PRX_SM_UT port_receive_sm_ut)
set(BDM_SM_UT bridge_detection_sm_ut)
set(PPM_SM_UT port_protocol_migration_sm_ut)
set(PTX_SM_UT port_transmit_sm_ut)

file(GLOB SOURCES
    ${PTI_SM_UT}.cpp
    ${PRX_SM_UT}.cpp
    ${BDM_SM_UT}.cpp
    ${PPM_SM_UT}.cpp
    ${PTX_SM_UT}.cpp
)

set(GTEST_LIB_DEPENDS
    /usr/local/lib/libgtest.a
    /usr/local/lib/libgtest_main.a
    /usr/local/lib/libgmock.a
)

add_executable(${PTI_SM_UT} ${STP_SOURCE} ${PTI_SM_UT}.cpp)
target_link_libraries(${PTI_SM_UT} ${GTEST_LIB_DEPENDS})

add_executable(${PRX_SM_UT} ${STP_SOURCE} ${PRX_SM_UT}.cpp)
target_link_libraries(${PRX_SM_UT} ${GTEST_LIB_DEPENDS})

add_executable(${BDM_SM_UT} ${STP_SOURCE} ${BDM_SM_UT}.cpp)
target_link_libraries(${BDM_SM_UT} ${GTEST_LIB_DEPENDS})

add_executable(${PPM_SM_UT} ${STP_SOURCE} ${PPM_SM_UT}.cpp)
target_link_libraries(${PPM_SM_UT} ${GTEST_LIB_DEPENDS})

add_executable(${PTX_SM_UT} ${STP_SOURCE} ${PTX_SM_UT}.cpp)
target_link_libraries(${PTX_SM_UT} ${GTEST_LIB_DEPENDS})

add_test(PortTimers ${PTI_SM_UT})
add_test(PortReceive ${PRX_SM_UT})
add_test(BridgeDetection ${BDM_SM_UT})
add_test(PortProtocolMigration ${PPM_SM_UT})
add_test(PortTransmit ${PTX_SM_UT})

#file(GLOB SOURCES
#    bpdu_data.hpp
#    bridge_id_ut.cpp
#    mac_ut.cpp
#    path_cost_ut.cpp
#    port_id_ut.cpp
#)

#include_directories( . )

#add_executable(bpdu_ut ${RSTP_SOURCES} bpdu_ut.cpp)
#target_link_libraries(bpdu_ut
#        gtest
#        gtest_main
#        gmock
#)

#add_executable(bridge_id_ut ${RSTP_SOURCES} bridge_id_ut.cpp)
#target_link_libraries(bridge_id_ut
#        gtest
#        gtest_main
#        gmock
#)

#add_executable(mac_ut ${RSTP_SOURCES} mac_ut.cpp)
#target_link_libraries(mac_ut
#        gtest
#        gtest_main
#        gmock
#)

#add_executable(path_cost_ut ${RSTP_SOURCES} path_cost_ut.cpp)
#target_link_libraries(path_cost_ut
#        gtest
#        gtest_main
#        gmock
#)

#add_executable(priority_vector_ut ${RSTP_SOURCES} priority_vector_ut.cpp)
#target_link_libraries(priority_vector_ut
#        gtest
#        gtest_main
#        gmock
#)

#add_executable(port_id_ut ${RSTP_SOURCES} port_id_ut.cpp)
#target_link_libraries(port_id_ut
#        gtest
#        gtest_main
#        gmock
#)

#add_test(BridgeId bridge_id_ut)
#add_test(Mac mac_ut)
#add_test(PortId port_id_ut)
