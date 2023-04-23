set(THIS "UtilitiesLib")
project(${THIS} VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/ConfigManager.cpp
    src/ErrorHandling.cpp
    src/TimeMeasurement.cpp
    src/TraceLog.cpp
    src/Profiler.cpp)

add_library(${THIS} STATIC ${SOURCES})

target_include_directories(
    ${THIS}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)