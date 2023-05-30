

add_subdirectory(array)
add_subdirectory(var)
add_subdirectory(dsl)

add_unit_test(
    storage_test
    GTEST
    FILES
        big_integer/detail/storage.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    storage_plus_test
    GTEST
    FILES
        big_integer/detail/plus.cpp
        big_integer/detail/minus.cpp
        big_integer/detail/negate.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    storage_bitwise_test
    GTEST
    FILES
        big_integer/detail/bit_and.cpp
        big_integer/detail/bit_or.cpp
        big_integer/detail/bit_xor.cpp
        big_integer/detail/bit_not.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    storage_shift_test
    GTEST
    FILES
        big_integer/detail/shift.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    storage_multiplies_test
    GTEST
    FILES
        big_integer/detail/multiplies.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    storage_compare_test
    GTEST
    FILES
        big_integer/detail/compare.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    storage_divides_test
    GTEST
    FILES
        big_integer/detail/divides.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

## Google tests
#add_executable(gtests
##    var.cpp
##    invoke.cpp
##    dsl.cpp
##    array.cpp
##    big_integer.cpp
##    dsl/detail/checked.cpp
##    dsl/add.cpp
##    dsl/minus.cpp
##    dsl/abs.cpp
##    dsl/bitwise_and.cpp
##    dsl/bitwise_or.cpp
##    dsl/bitwise_xor.cpp
##    dsl/bitwise_invert.cpp
##    dsl/divide.cpp
##    dsl/mask.cpp
##    dsl/intersection.cpp
##    dsl/is_equal.cpp
##    dsl/is_subset.cpp
#
#)
#
#target_link_libraries(gtests PUBLIC rapidcheck PRIVATE safe_arithmetic gmock gtest gmock_main )
#target_include_directories(gtests PRIVATE ${rapidcheck_SOURCE_DIR}/extras/gtest/include)
#
#include(GoogleTest)
#gtest_discover_tests(gtests)