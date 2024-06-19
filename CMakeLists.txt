cmake_minimum_required(VERSION 3.22)

set(THIS UtilitiesLib)
project(${THIS} VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/ConfigManager.cpp
    src/ErrorHandling.cpp
    #src/TimeMeasurement.cpp
    src/TraceLog.cpp
    src/Profiler.cpp
    src/ThreadWatcher.cpp)

add_library(${THIS} STATIC ${SOURCES})

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

##################
#-- Unit Tests --#
##################

find_package(GTest)

if(NOT GTest_FOUND)
    return()
endif()

set(THIS_TEST ${THIS}Test)

add_executable(${THIS_TEST}
    tests/SplitString.cpp)

target_link_libraries(${THIS_TEST} PRIVATE
    GTest::gtest
    GTest::gtest_main
    ${THIS})