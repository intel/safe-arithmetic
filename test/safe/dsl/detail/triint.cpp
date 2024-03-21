#include <safe/big_integer.hpp>
#include <safe/big_integer_gen.hpp>
#include <safe/dsl/detail/triint.hpp>
#include <safe/dsl/detail/triint_gen.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::dsl::detail {
// template <std::size_t NumBits> using big_triint = triint<big_integer<NumBits>>;

// RC_GTEST_PROP(triint, bit_and_is_commutative,
//               (big_triint<256> a, big_triint<256> b)) {
//     RC_ASSERT((a & b) == (b & a));
// }

// RC_GTEST_PROP(triint, bit_and_is_associative,
//               (big_triint<256> a, big_triint<256> b, big_triint<256> c)) {
//     RC_ASSERT(((a & b) & c) == (a & (b & c)));
// }

// RC_GTEST_PROP(triint, bitwise_demorgan_laws,
//               (big_triint<256> a, big_triint<256> b)) {
//     RC_ASSERT(~(a | b) == (~a & ~b));
//     RC_ASSERT(~(a & b) == (~a | ~b));
// }
} // namespace safe::dsl::detail