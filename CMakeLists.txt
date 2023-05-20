cmake_minimum_required(VERSION 3.16)
project(safe_arithmetic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(cmake/CPM.cmake)
CPMAddPackage("gh:boostorg/mp11#boost-1.80.0")
CPMAddPackage("gh:google/googletest#release-1.12.1")

include(CTest)
enable_testing()

add_subdirectory(include)
add_subdirectory(test)
add_subdirectory(examples)


