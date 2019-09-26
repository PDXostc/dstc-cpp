cmake_minimum_required(VERSION 3.10)

option(BUILD_TESTS "Builds test binaries" ON)

# Main library
file(GLOB lib_SRC
    "./*.cpp"
)

add_library(dstcpp SHARED ${lib_SRC})

foreach( H_HEADER dstc reliable_multicast)
    set (H_HEADER_PATH "VAR_${H_HEADER}")
    set (H_FILE "${H_HEADER}.h")
    find_path(${H_HEADER_PATH} ${H_FILE} HINTS ${EXTRA_SEARCH_PATH})
    if (NOT ${H_HEADER_PATH})
        message(FATAL_ERROR "${H_FILE} not found")
    endif()
    target_include_directories(dstcpp PUBLIC ${${H_HEADER_PATH}})
endforeach( H_HEADER )

set(DEPEND_LIBS_LIST)

foreach ( DEPEND_LIB libdstc.so librmc.so)
    set ( DEPEND_LIB_PATH "VAR_${DEPEND_LIB}" )
    find_library(${DEPEND_LIB_PATH} ${DEPEND_LIB} HINTS ${EXTRA_SEARCH_PATH})
    if (NOT ${DEPEND_LIB_PATH})
        message(FATAL_ERROR "${DEPEND_LIB} not found")
    endif()
    list(APPEND DEPEND_LIBS_LIST ${${DEPEND_LIB_PATH}})
endforeach ( DEPEND_LIB )

target_link_libraries(dstcpp ${DEPEND_LIBS_LIST})

set_property(TARGET dstcpp PROPERTY CXX_STANDARD 17)

# Main Test Executable

if (BUILD_TESTS)

    file(GLOB test_SRC
        "./test/*.cpp"
    )

    add_executable(run_tests ${test_SRC})
    target_link_libraries(run_tests dstcpp gtest pthread)
    target_include_directories(run_tests PRIVATE .)

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
                    array_of_struct_server
                    mixed_types_server
                    callback_server
                    )

    foreach (SERVER IN LISTS SERVER_NAMES)
        add_executable(${SERVER} test/${SERVER}.c)
        target_link_libraries(${SERVER} ${DEPEND_LIBS_LIST})
    endforeach()
endif()