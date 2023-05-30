#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include <safe/big_integer/interface/big_integer_gen.hpp>

#include <safe/big_integer/interface/big_integer.hpp>


namespace safe::_big_integer::interface {
    RC_GTEST_PROP(big_integer, is_plus_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a + b) == big_integer<64>(big_integer<64>(a) + big_integer<64>(b)));
    }    
    
    RC_GTEST_PROP(big_integer, plus_is_commutative, (big_integer<256> a, big_integer<256> b)) {
        RC_ASSERT((a + b) == (b + a));
    }  
    
    RC_GTEST_PROP(big_integer, plus_is_associative, (big_integer<256> a, big_integer<256> b, big_integer<256> c)) {
        RC_ASSERT(((a + b) + c) == (a + (b + c)));
    }
    
    RC_GTEST_PROP(big_integer, plus_identity, (big_integer<256> a)) {
        RC_ASSERT((a + 0) == a);
    }
    
    RC_GTEST_PROP(big_integer, plus_two, (big_integer<256> a)) {
        RC_ASSERT(((a + 1) + 1) == (a + 2));
    }
    
    RC_GTEST_PROP(big_integer, minus_itself_is_zero, (big_integer<256> a)) {
        RC_ASSERT((a - a) == 0);
    }

    RC_GTEST_PROP(big_integer, multiply_is_commutative, (big_integer<256> a, big_integer<256> b)) {
        RC_ASSERT((a * b) == (b * a));
    }

    RC_GTEST_PROP(big_integer, multiply_is_associative, (big_integer<256> a, big_integer<256> b, big_integer<256> c)) {
        RC_ASSERT(((a * b) * c) == (a * (b * c)));
    }

    RC_GTEST_PROP(big_integer, multiply_identity, (big_integer<256> a)) {
        RC_ASSERT((a * 1) == a);
    }

    RC_GTEST_PROP(big_integer, multiply_by_zero_is_zero, (big_integer<256> a)) {
        RC_ASSERT((a * 0) == 0);
    }

    RC_GTEST_PROP(big_integer, divmod_and_multiply_relation, (big_integer<256> dividend, big_integer<256> divisor)) {
        RC_PRE(divisor != 0);
        auto [q, r] = divmod(dividend, divisor);
        RC_ASSERT(((divisor * q) + r) == dividend);
    }

    RC_GTEST_PROP(big_integer, division_remainder_and_multiply_relation, (big_integer<256> dividend, big_integer<256> divisor)) {
        RC_PRE(divisor != 0);
        RC_ASSERT(((divisor * (dividend / divisor)) + (dividend % divisor)) == dividend);
    }

    RC_GTEST_PROP(big_integer, division_identity, (big_integer<256> a)) {
        RC_ASSERT((a / 1) == a);
    }

    RC_GTEST_PROP(big_integer, division_zero_dividend, (big_integer<256> a)) {
        RC_PRE(a != 0);
        RC_ASSERT((0 / a) == 0);
    }

    TEST(big_integer, plus) {
        using T = big_integer<32>;
        ASSERT_EQ(T(1) + T(2), T(3));
    }
}