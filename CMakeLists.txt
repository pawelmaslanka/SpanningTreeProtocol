cmake_minimum_required(VERSION 3.0)

project(SpanningTreeProtocol)

set(CMAKE_BUILD_TYPE Debug)

# Set toolchain flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. \
                         Please use a different C++ compiler.")
endif()

#if(CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
#endif()

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE})
file(GLOB_RECURSE PROJECT_INCLUDE "${INCLUDE}/stp/*.hpp" "${INCLUDE}/stp/sm/*.hpp")

set(SOURCE ${CMAKE_SOURCE_DIR}/source)

file(GLOB STP_SOURCE
    ${SOURCE}/bridge_detection_sm.cpp
    ${SOURCE}/port_information_sm.cpp
    ${SOURCE}/port_protocol_migration_sm.cpp
    ${SOURCE}/port_receive_sm.cpp
    ${SOURCE}/port_timers_sm.cpp
    ${SOURCE}/port_transmit_sm.cpp
    ${SOURCE}/port_role_selection_sm.cpp
    ${SOURCE}/port_role_transitions_sm.cpp
    ${SOURCE}/port_state_transition_sm.cpp
    ${SOURCE}/topology_change_sm.cpp
    ${SOURCE}/bpdu.cpp
    ${SOURCE}/bridge.cpp
    ${SOURCE}/bridge_id.cpp
    ${SOURCE}/logger.cpp
    ${SOURCE}/mac.cpp
    ${SOURCE}/management.cpp
    ${SOURCE}/path_cost.cpp
    ${SOURCE}/port.cpp
    ${SOURCE}/port_id.cpp
    ${SOURCE}/priority_vector.cpp
    ${SOURCE}/sm_conditions.cpp
    ${SOURCE}/sm_procedures.cpp
    ${SOURCE}/state_machine.cpp
    ${SOURCE}/time.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_INCLUDE} ${STP_SOURCE} ${SOURCE}/main.cpp)

add_subdirectory(test)
