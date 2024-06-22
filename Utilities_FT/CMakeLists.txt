set(THIS UtilitiesFT)
set(ARGUMENTS ${THIS}_Arguments)

set(CMAKE_CXX_STANDARD 20)

add_executable(${ARGUMENTS}
    src/main01_arguments.cpp)

target_link_libraries(${ARGUMENTS} PRIVATE UtilitiesLib)