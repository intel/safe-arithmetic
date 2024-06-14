#include <safe/dsl/detail/triint.hpp>
#include <safe/dsl/detail/triint_gen.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::dsl::detail {
using big_triint = triint<uint64_t>;

RC_GTEST_PROP(triint, bit_and_is_commutative,
              (big_triint a, big_triint b)) {
    RC_ASSERT((a & b) == (b & a));
}

RC_GTEST_PROP(triint, bit_and_is_associative,
              (big_triint a, big_triint b, big_triint c)) {
    RC_ASSERT(((a & b) & c) == (a & (b & c)));
}

RC_GTEST_PROP(triint, bitwise_demorgan_laws,
              (big_triint a, big_triint b)) {
    RC_ASSERT(~(a | b) == (~a & ~b));
    RC_ASSERT(~(a & b) == (~a | ~b));
}
} // namespace safe::dsl::detail