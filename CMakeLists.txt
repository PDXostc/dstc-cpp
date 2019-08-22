

add_library(dstcpp SHARED dstccallback.cpp)

include_directories(dstcpp ../dstc)
include_directories(dstcpp ../reliable_multicast)

target_link_libraries(dstcpp dstc)
target_link_libraries(dstcpp rmc)

set_property(TARGET dstcpp PROPERTY CXX_STANDARD 17)

file(GLOB test_SRC
    "./test/*.cpp"
)

add_executable(run_tests ${test_SRC})
target_link_libraries(run_tests dstcpp gtest pthread)

set_property(TARGET run_tests PROPERTY CXX_STANDARD 17)