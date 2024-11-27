function(add_fail_tests)
    foreach(name ${ARGN})
        add_compile_fail_test("${name}.cpp" LIBRARIES safe_arithmetic)
    endforeach()
endfunction()

add_fail_tests(
    incompatible_sign_cast
    incompatible_range_cast)
