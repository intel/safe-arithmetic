cmake_minimum_required(VERSION 3.25)

project(safe_arithmetic LANGUAGES CXX)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
            "In-source builds are a bad idea. Please make a build directory instead."
    )
endif()

include(cmake/dependencies.cmake)
include(cmake/libraries.cmake)
include(cmake/quality.cmake)

add_versioned_package("gh:boostorg/mp11#boost-1.83.0")

add_library(safe_arithmetic INTERFACE)
target_compile_features(safe_arithmetic INTERFACE cxx_std_20)
target_include_directories(safe_arithmetic INTERFACE include)
target_link_libraries(safe_arithmetic INTERFACE boost_mp11)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    clang_tidy_interface(safe_arithmetic)
    add_subdirectory(test)
endif()
