# Test that code is NOT able to compile
function(test_static_assert_fail TEST_NAME)
    add_executable(${TEST_NAME}_t EXCLUDE_FROM_ALL ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME}_t PRIVATE safe_arithmetic)
    add_test(NAME ${TEST_NAME}
             COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
                     ${TEST_NAME}_t)
    set_tests_properties(
        ${TEST_NAME} PROPERTIES PASS_REGULAR_EXPRESSION
                                "(static_assert)|(static assertion failed)")
endfunction()

# Test that code that must compile
function(test_static_assert_pass TEST_NAME)
    add_executable(${TEST_NAME}_t EXCLUDE_FROM_ALL ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME}_t PRIVATE safe_arithmetic)
    add_test(NAME ${TEST_NAME}
             COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
                     ${TEST_NAME}_t)
endfunction()

add_subdirectory(safe)
