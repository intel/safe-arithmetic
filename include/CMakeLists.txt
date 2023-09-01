add_library(safe_arithmetic INTERFACE)
target_include_directories(safe_arithmetic
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(safe_arithmetic INTERFACE boost_mp11)
