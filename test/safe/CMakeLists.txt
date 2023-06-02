

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


add_unit_test(
    big_integer_test
    GTEST
    FILES
        big_integer.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    var_test
    GTEST
    FILES
        var.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    match_test
    GTEST
    FILES
        match.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    array_test
    GTEST
    FILES
        array.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    dsl_add_test
    GTEST
    FILES
        dsl/add.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    dsl_divide_test
    GTEST
    FILES
        dsl/divide.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)

add_unit_test(
    dsl_intersection_test
    GTEST
    FILES
        dsl/intersection.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)      

add_unit_test(
    dsl_is_equal_test
    GTEST
    FILES
        dsl/is_equal.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)    
    

add_unit_test(
    dsl_is_subset_test
    GTEST
    FILES
        dsl/is_subset.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)   

add_unit_test(
    dsl_abs_test
    GTEST
    FILES
        dsl/abs.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  
  
add_unit_test(
    dsl_mask_test
    GTEST
    FILES
        dsl/mask.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  

add_unit_test(
    dsl_bitwise_and_test
    GTEST
    FILES
        dsl/bitwise_and.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  


add_unit_test(
    dsl_detail_triint_test
    GTEST
    FILES
        dsl/detail/triint.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  

add_unit_test(
    dsl_bitwise_or_test
    GTEST
    FILES
        dsl/bitwise_or.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  

add_unit_test(
    dsl_bitwise_xor_test
    GTEST
    FILES
        dsl/bitwise_xor.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  

add_unit_test(
    dsl_bitwise_invert_test
    GTEST
    FILES
        dsl/bitwise_invert.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  

add_unit_test(
    dsl_test
    GTEST
    FILES
        dsl.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  

add_unit_test(
    dsl_minus_test
    GTEST
    FILES
        dsl/minus.cpp
    INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/test/
    LIBRARIES
        safe_arithmetic)  
