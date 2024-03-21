add_compile_fail_test(assign_constant_max_violation.cpp LIBRARIES
                      safe_arithmetic)
add_compile_fail_test(assign_constant_min_violation.cpp LIBRARIES
                      safe_arithmetic)
add_compile_fail_test(construct_constant_max_violation.cpp LIBRARIES
                      safe_arithmetic)
add_compile_fail_test(construct_constant_min_violation.cpp LIBRARIES
                      safe_arithmetic)
add_compile_fail_test(interval_larger_than_type.cpp LIBRARIES safe_arithmetic)
add_compile_fail_test(assign_lhs_rhs_union.cpp LIBRARIES safe_arithmetic)
add_compile_fail_test(assign_lhs_union.cpp LIBRARIES safe_arithmetic)
add_compile_fail_test(assign_rhs_union.cpp LIBRARIES safe_arithmetic)
