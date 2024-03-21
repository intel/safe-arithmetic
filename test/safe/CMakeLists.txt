add_subdirectory(array)
add_subdirectory(constrained_number)

function(add_test_suites)
    foreach(test_file ${ARGN})
        string(REPLACE "/" "_" test_name "${test_file}")

        add_unit_test(
            ${test_name}
            GTEST
            FILES
            ${test_file}
            INCLUDE_DIRECTORIES
            ${CMAKE_SOURCE_DIR}/test/
            LIBRARIES
            safe_arithmetic)
    endforeach()
endfunction()

add_test_suites(
    constrained_number.cpp
    match.cpp
    array.cpp
    dsl/add.cpp
    dsl/divide.cpp
    dsl/intersection.cpp
    dsl/is_equal.cpp
    dsl/is_subset.cpp
    dsl/abs.cpp
    dsl/constrain_mask.cpp
    dsl/bitwise_and.cpp
    dsl/detail/triint.cpp
    dsl/bitwise_or.cpp
    dsl/bitwise_xor.cpp
    dsl/bitwise_invert.cpp
    dsl.cpp
    dsl/minus.cpp)
