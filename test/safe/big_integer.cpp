#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include <safe/big_integer_gen.hpp>
#include <safe/big_integer.hpp>


namespace safe {
    RC_GTEST_PROP(big_integer, is_plus_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a + b) == big_integer<64>(big_integer<64>(a) + big_integer<64>(b)));
    }    

    RC_GTEST_PROP(big_integer, is_minus_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a - b) == big_integer<64>(big_integer<64>(a) - big_integer<64>(b)));
    }    

    RC_GTEST_PROP(big_integer, is_multiply_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a * b) == big_integer<64>(big_integer<64>(a) * big_integer<64>(b)));
    }     

    RC_GTEST_PROP(big_integer, is_divide_64, (int64_t a, int64_t b)) {
        RC_PRE(b != 0);
        RC_ASSERT(big_integer<64>(a / b) == big_integer<64>(big_integer<64>(a) / big_integer<64>(b)));
    }    

    RC_GTEST_PROP(big_integer, is_mod_64, (int64_t a, int64_t b)) {
        RC_PRE(b != 0);
        RC_ASSERT(big_integer<64>(a % b) == big_integer<64>(big_integer<64>(a) % big_integer<64>(b)));
    }    

    RC_GTEST_PROP(big_integer, is_bit_or_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a | b) == (big_integer<64>(a) | big_integer<64>(b)));
    }   

    RC_GTEST_PROP(big_integer, is_bit_and_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a & b) == (big_integer<64>(a) & big_integer<64>(b)));
    }   

    RC_GTEST_PROP(big_integer, is_bit_xor_64, (int64_t a, int64_t b)) {
        RC_ASSERT(big_integer<64>(a ^ b) == (big_integer<64>(a) ^ big_integer<64>(b)));
    }   

    RC_GTEST_PROP(big_integer, is_bit_shift_left_64, (int64_t a)) {
        auto const i = *rc::gen::inRange<int32_t>(0, 63);
        RC_ASSERT(big_integer<64>(a << i) == (big_integer<64>(a) << i));
    }   

    RC_GTEST_PROP(big_integer, is_bit_shift_right_64, (int64_t a)) {
        auto const i = *rc::gen::inRange<int32_t>(0, 63);
        RC_ASSERT(big_integer<64>(a >> i) == (big_integer<64>(a) >> i));
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

    RC_GTEST_PROP(big_integer, shift_left_and_right, (big_integer<256> a)) {
        big_integer<512> b{a};

        auto const i = *rc::gen::inRange<int32_t>(0, 256);
        RC_ASSERT(b == ((b << i) >> i));
    }

    RC_GTEST_PROP(big_integer, shift_distributes_over_add, (big_integer<256> a, big_integer<256> b)) {
        big_integer<512> a_prime{a};
        big_integer<512> b_prime{b};

        auto const i = *rc::gen::inRange<int32_t>(0, 256);

        RC_ASSERT(((a_prime + b_prime) << i) == ((a_prime << i) + (b_prime << i)));
    }

    RC_GTEST_PROP(big_integer, bitwise_demorgan_laws, (big_integer<512> a, big_integer<512> b)) {
        RC_ASSERT(~(a | b) == (~a & ~b));
        RC_ASSERT(~(a & b) == (~a | ~b));
    }

    void shifted_operation(auto op, big_integer<64> small_a, big_integer<64> small_b) {
        big_integer<1024> a{small_a};
        big_integer<1024> b{small_b};

        auto const i = *rc::gen::inRange<int32_t>(0, 959);

        RC_ASSERT(op(a, b) == op((a << i), (b << i)) >> i);
    }
    
    RC_GTEST_PROP(big_integer, shifted_bit_and, (big_integer<64> small_a, big_integer<64> small_b)) {
        shifted_operation(std::bit_and{}, small_a, small_b);
    }  
    
    RC_GTEST_PROP(big_integer, shifted_bit_or, (big_integer<64> small_a, big_integer<64> small_b)) {
        shifted_operation(std::bit_or{}, small_a, small_b);
    }  
    
    RC_GTEST_PROP(big_integer, shifted_bit_xor, (big_integer<64> small_a, big_integer<64> small_b)) {
        shifted_operation(std::bit_xor{}, small_a, small_b);
    }  
    
    RC_GTEST_PROP(big_integer, shifted_plus, (big_integer<64> small_a, big_integer<64> small_b)) {
        shifted_operation(std::plus{}, small_a, small_b);
    }  
    
    RC_GTEST_PROP(big_integer, shifted_minus, (big_integer<64> small_a, big_integer<64> small_b)) {
        shifted_operation(std::minus{}, small_a, small_b);
    }  


    TEST(big_integer, plus) {
        using T = big_integer<32>;
        ASSERT_EQ(T(1) + T(2), T(3));
    }
}