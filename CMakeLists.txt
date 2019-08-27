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

set (SERVER_NAMES print_name_and_age_server
                  basic_type_one_arg_server
                  basic_type_many_args_server
                  array_type_one_arg_server
                  array_type_many_args_server
                  dynamic_type_server
                  multiple_dynamic_type_server
                  struct_type_server
                  multiple_struct_type_server
                  )

foreach (SERVER IN LISTS SERVER_NAMES)
    add_executable(${SERVER} test/${SERVER}.c)
    target_link_libraries(${SERVER} dstc)
    target_link_libraries(${SERVER} rmc)
endforeach()