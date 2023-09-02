# Test that code is NOT able to compile
function(test_static_assert_fail test_file)
    string(REPLACE "/" "_" test_name "${test_file}")
    string(PREPEND test_name "EXPECT_FAIL.")
    add_executable(${test_name} EXCLUDE_FROM_ALL ${test_file})
    target_link_libraries(${test_name} PRIVATE safe_arithmetic)
    add_test(NAME ${test_name}
             COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
                     ${test_name})
    set_tests_properties(
        ${test_name} PROPERTIES PASS_REGULAR_EXPRESSION
                                "(static_assert)|(static assertion failed)")
endfunction()

add_subdirectory(safe)
