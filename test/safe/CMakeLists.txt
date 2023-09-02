add_subdirectory(array)
add_subdirectory(var)

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
    big_integer/detail/storage.cpp
    big_integer/detail/plus.cpp
    big_integer/detail/minus.cpp
    big_integer/detail/negate.cpp
    big_integer/detail/bit_and.cpp
    big_integer/detail/bit_or.cpp
    big_integer/detail/bit_xor.cpp
    big_integer/detail/bit_not.cpp
    big_integer/detail/shift.cpp
    big_integer/detail/multiplies.cpp
    big_integer/detail/compare.cpp
    big_integer/detail/divides.cpp
    big_integer.cpp
    var.cpp
    match.cpp
    array.cpp
    dsl/add.cpp
    dsl/divide.cpp
    dsl/intersection.cpp
    dsl/is_equal.cpp
    dsl/is_subset.cpp
    dsl/abs.cpp
    dsl/mask.cpp
    dsl/bitwise_and.cpp
    dsl/detail/triint.cpp
    dsl/bitwise_or.cpp
    dsl/bitwise_xor.cpp
    dsl/bitwise_invert.cpp
    dsl.cpp
    dsl/minus.cpp)
