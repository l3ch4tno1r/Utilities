set(TEST UtilitiesUT)
set(CMAKE_CXX_STANDARD 20)

find_package(GTest)

if(NOT GTest_FOUND)
    return()
endif()

add_executable(${TEST}
    src/Arguments.cpp
    src/BitwiseEditor.cpp
    src/SplitString.cpp
    src/Units.cpp)

target_link_libraries(${TEST} PRIVATE
    GTest::gtest
    GTest::gtest_main
    UtilitiesLib)