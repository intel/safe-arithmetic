cmake_minimum_required(VERSION 3.25)

project(safe_arithmetic LANGUAGES CXX)

include(cmake/get_cpm.cmake)
cpmaddpackage("gh:intel/cicd-repo-infrastructure#main")

add_versioned_package("gh:boostorg/mp11#boost-1.83.0")

add_library(safe_arithmetic INTERFACE)
target_compile_features(safe_arithmetic INTERFACE cxx_std_20)
target_include_directories(safe_arithmetic INTERFACE include)
target_link_libraries_system(safe_arithmetic INTERFACE boost_mp11)

if(PROJECT_IS_TOP_LEVEL)
    add_docs(docs)
    clang_tidy_interface(safe_arithmetic)
    add_subdirectory(test)
endif()
