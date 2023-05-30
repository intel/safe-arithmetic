cmake_minimum_required(VERSION 3.16)
project(safe_arithmetic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(cmake/CPM.cmake)

include(cmake/libraries.cmake)
include(cmake/dependencies.cmake)
include(cmake/test.cmake)
include(cmake/sanitizers.cmake)
include(CTest)
enable_testing()

add_versioned_package("gh:boostorg/mp11#boost-1.80.0")

add_subdirectory(include)
add_subdirectory(test)
add_subdirectory(examples)


