cmake_minimum_required(VERSION 3.10)

# Main library

file(GLOB lib_SRC
    "./*.cpp"
)

add_library(dstcpp SHARED ${lib_SRC})

include_directories(dstcpp ../dstc)
include_directories(dstcpp ../reliable_multicast)

target_link_libraries(dstcpp dstc)
target_link_libraries(dstcpp rmc)

set_property(TARGET dstcpp PROPERTY CXX_STANDARD 17)


# Main Test Executable

file(GLOB test_SRC
    "./test/*.cpp"
)

add_executable(run_tests ${test_SRC})
target_link_libraries(run_tests dstcpp gtest pthread)
include_directories(run_tests .)

set_property(TARGET run_tests PROPERTY CXX_STANDARD 17)

# DSTC Test Servers

add_executable(print_name_and_age_server test/print_name_and_age_server.c)
target_link_libraries(print_name_and_age_server dstc)
target_link_libraries(print_name_and_age_server rmc)

