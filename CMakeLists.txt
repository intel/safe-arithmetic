cmake_minimum_required(VERSION 3.16)
project(safe_contracts LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(CTest)
enable_testing()

add_subdirectory(include)
add_subdirectory(test)
