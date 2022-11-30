add_library(safe_arithmetic INTERFACE safe/detail/fwd.hpp safe/detail/integral_type.hpp safe/detail/var_assign_static_assert.hpp safe/detail/make_constant.hpp)
target_include_directories(safe_arithmetic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(safe_arithmetic INTERFACE boost_mp11)
