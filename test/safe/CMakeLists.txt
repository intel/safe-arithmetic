
add_subdirectory(array)
add_subdirectory(var)
add_subdirectory(dsl)


# Google tests
add_executable(gtests
    var.cpp
    invoke.cpp
    dsl.cpp
    array.cpp
    runtime_check.cpp
    checked.cpp
    dsl/add.cpp
    dsl/abs.cpp
    dsl/bitwise_and.cpp
    dsl/bitwise_or.cpp
    dsl/bitwise_xor.cpp
    dsl/bitwise_invert.cpp
    dsl/divide.cpp
    dsl/mask.cpp
    dsl/intersection.cpp)

target_link_libraries(gtests PRIVATE safe_arithmetic gmock gtest gmock_main)

include(GoogleTest)
gtest_discover_tests(gtests)