
add_subdirectory(array)
add_subdirectory(var)
add_subdirectory(dsl)


# Google tests
add_executable(gtests var.cpp invoke.cpp dsl.cpp array.cpp runtime_check.cpp checked.cpp)

target_link_libraries(gtests PRIVATE safe_arithmetic gmock gtest gmock_main)

include(GoogleTest)
gtest_discover_tests(gtests)