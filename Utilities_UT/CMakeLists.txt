set(THIS_TEST UtilitiesTest)
set(CMAKE_CXX_STANDARD 20)

find_package(GTest)

if(NOT GTest_FOUND)
    return()
endif()

add_executable(${THIS_TEST}
    src/SplitString.cpp)

target_link_libraries(${THIS_TEST} PRIVATE
    GTest::gtest
    GTest::gtest_main
    UtilitiesLib)