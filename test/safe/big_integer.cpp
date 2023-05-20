#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe/big_integer.hpp>


using namespace safe::literals;


TEST(big_integer, default_construction) {
    safe::big_integer<32> v{};
}

TEST(big_integer, equality) {
    safe::big_integer<32> lhs{};
    safe::big_integer<32> rhs{};
    ASSERT_EQ(lhs, rhs);
}

TEST(big_integer, construct_from_int) {
    safe::big_integer<32> lhs{42u};
    safe::big_integer<32> rhs{42u};
    ASSERT_EQ(lhs, rhs);

    safe::big_integer<32> zero{};
    ASSERT_NE(lhs, zero);
}

TEST(big_integer, add_two) {
    safe::big_integer<32> lhs{13u};
    safe::big_integer<32> rhs{24u};
    safe::big_integer<32> result{37u};
    ASSERT_EQ(lhs + rhs, result);
}

TEST(big_integer, ctad_int) {
    auto lhs = safe::big_integer{42u};
    auto rhs = safe::big_integer{42u};
    ASSERT_EQ(lhs, rhs);

    auto zero = safe::big_integer{0u};
    ASSERT_NE(lhs, zero);
}

TEST(big_integer, ctad_long_long) {
    auto lhs = safe::big_integer{0xffff'ffff'ffff'ffffull};
    auto rhs = safe::big_integer{0xffff'ffff'ffff'ffffull};
    ASSERT_EQ(lhs, rhs);

    auto zero = safe::big_integer{0u};
    ASSERT_NE(lhs, zero);
}

TEST(big_integer, trivial_multiply) {
    auto lhs = safe::big_integer{1u};
    auto rhs = safe::big_integer{1u};
    ASSERT_EQ(lhs * rhs, rhs);

    auto zero = safe::big_integer{0u};
    ASSERT_EQ(lhs * zero, zero);
}

TEST(big_integer, multiply_32_bit) {
    auto lhs = safe::big_integer{0xffff'ffffu};
    auto rhs = safe::big_integer{0xffff'ffffu};
    auto result = safe::big_integer{0xffff'fffe'0000'0001ull};
    ASSERT_EQ(lhs * rhs, result);
}

TEST(big_integer, udl_init) {
    ASSERT_EQ(1_i, safe::big_integer{1u});
    ASSERT_EQ(12_i, safe::big_integer{12u});
    ASSERT_EQ(31415926_i, safe::big_integer{31415926u});
}

TEST(big_integer, udl_seperation_char) {
    ASSERT_EQ(1'000_i, 1000_i);
}

TEST(big_integer, big_mult) {
    auto v = 10'000'000'000_i;
    auto r = 100'000'000'000'000'000'000_i;

    ASSERT_EQ(v * v, r);

    ASSERT_EQ(
        78546789034526897234986_i * 345962345792345657894324423_i,
        27174231388841418566038694457802460003461149863078_i);
}

TEST(big_integer, udl_init_hex) {
    ASSERT_EQ(0x0_i, safe::big_integer{0x0u});
    ASSERT_EQ(0x1_i, safe::big_integer{0x1u});
    ASSERT_EQ(0x9_i, safe::big_integer{0x9u});
    ASSERT_EQ(0xa_i, safe::big_integer{0xau});
    ASSERT_EQ(0xA_i, safe::big_integer{0xAu});
    ASSERT_EQ(0xf_i, safe::big_integer{0xfu});
    ASSERT_EQ(0xF_i, safe::big_integer{0xFu});
    ASSERT_EQ(0x12_i, safe::big_integer{0x12u});
    ASSERT_EQ(0x3141'5926_i, safe::big_integer{0x31415926u});
    ASSERT_EQ(0xc001'ba11_i, safe::big_integer{0xc001'ba11u});
    ASSERT_EQ(0xC001'BA11_i, safe::big_integer{0xC001'BA11u});
}

TEST(big_integer, udl_init_bin) {
    ASSERT_EQ(0b0_i, safe::big_integer{0b0u});
    ASSERT_EQ(0b1_i, safe::big_integer{0b1u});
    ASSERT_EQ(0b11110000'11110000'11110000'11110000_i, safe::big_integer{0b11110000'11110000'11110000'11110000u});
}

TEST(big_integer, udl_init_oct) {
    ASSERT_EQ(010_i, safe::big_integer{010u});
    ASSERT_EQ(01234567_i, safe::big_integer{01234567u});
}

TEST(big_integer, bitwise_or) {
    ASSERT_EQ(0_i, 0_i | 0_i);
    ASSERT_EQ(1_i, 0_i | 1_i);
    ASSERT_EQ(1_i, 1_i | 0_i);
    ASSERT_EQ(1_i, 1_i | 1_i);

    ASSERT_EQ(0xffff'ffff'ffff'ffff_i, 1_i | 0xffff'ffff'ffff'fffe_i);
    ASSERT_EQ(0xffff'ffff'ffff'ffff_i, 0xffff'ffff'ffff'fffe_i | 1_i);
    ASSERT_EQ(0xffff'ffff'ffff'fffe_i, 0_i | 0xffff'ffff'ffff'fffe_i);
    ASSERT_EQ(0xffff'ffff'ffff'fffe_i, 0xffff'ffff'ffff'fffe_i | 0_i);

    ASSERT_EQ(0xffff'ffff_i, 0xffff'0000_i | 0x0000'ffff_i);

    ASSERT_EQ(0xffff'ffff'ffff'ffff'ffff'ffff'ffff'c001_i, 0xffff'ffff'ffff'ffff'ffff'ffff'ffff'0000_i | 0xc001_i);

    ASSERT_EQ(
        0xf00d'ffff'ffff'ffff'ffff'ffff'1234'c001_i,
        0xf000'ffff'ffff'ffff'ffff'ffff'1200'0000_i |
        0x000d'ffff'ffff'ffff'ffff'ffff'0034'c001_i);
}

TEST(big_integer, bitwise_and) {
    ASSERT_EQ(0_i, 0_i & 0_i);
    ASSERT_EQ(0_i, 0_i & 1_i);
    ASSERT_EQ(0_i, 1_i & 0_i);
    ASSERT_EQ(1_i, 1_i & 1_i);

    ASSERT_EQ(0_i, 1_i & 0xffff'ffff'ffff'fffe_i);
    ASSERT_EQ(0_i, 0xffff'ffff'ffff'fffe_i & 1_i);
    ASSERT_EQ(0_i, 0_i & 0xffff'ffff'ffff'fffe_i);
    ASSERT_EQ(0_i, 0xffff'ffff'ffff'fffe_i & 0_i);

    ASSERT_EQ(0_i, 0xffff'0000_i & 0x0000'ffff_i);

    ASSERT_EQ(0_i, 0xffff'ffff'ffff'ffff'ffff'ffff'ffff'0000_i & 0xc001_i);

    ASSERT_EQ(0xc001_i, 0xffffffff_i & 0xc001_i);
}

TEST(big_integer, bitwise_xor) {
    ASSERT_EQ(0_i, 0_i ^ 0_i);
    ASSERT_EQ(1_i, 0_i ^ 1_i);
    ASSERT_EQ(1_i, 1_i ^ 0_i);
    ASSERT_EQ(0_i, 1_i ^ 1_i);

    ASSERT_EQ(0xffff'ffff_i, 0xffff'0000_i ^ 0x0000'ffff_i);

    ASSERT_EQ(0xffff'ffff'ffff'ffff_i, 1_i ^ 0xffff'ffff'ffff'fffe_i);
    ASSERT_EQ(0xffff'ffff'ffff'ffff_i, 0xffff'ffff'ffff'fffe_i ^ 1_i);
    ASSERT_EQ(0xffff'ffff'ffff'fffe_i, 0_i ^ 0xffff'ffff'ffff'fffe_i);
    ASSERT_EQ(0xffff'ffff'ffff'fffe_i, 0xffff'ffff'ffff'fffe_i ^ 0_i);

    ASSERT_EQ(0xffff'ffff'ffff'fffe_i, 1_i ^ 0xffff'ffff'ffff'ffff_i);
    ASSERT_EQ(0xffff'ffff'ffff'fffe_i, 0xffff'ffff'ffff'ffff_i ^ 1_i);

    ASSERT_EQ(
        0xf00d'0000'ffff'ffff'0000'ffff'1234'c001_i,
        0xfff0'ffff'0000'ffff'ffff'0000'1200'0000_i ^
        0x0ffd'ffff'ffff'0000'ffff'ffff'0034'c001_i);
}

template<std::size_t N>
constexpr auto size_ = std::integral_constant<std::size_t, N>{};

TEST(big_integer, left_shift) {
    ASSERT_EQ(1_i, 1_i << 0_i);
    ASSERT_EQ(0x1'0000'0000_i, 1_i << 32_i);
    ASSERT_EQ(0x1'0000'0000'0000'0000_i, 1_i << 64_i);
    ASSERT_EQ(0x1'0000'0000'0000'0000'0000'0000_i, 1_i << 96_i);

    ASSERT_EQ(0b10_i, 1_i << 1_i);
    ASSERT_EQ(0x2'0000'0000_i, 1_i << 33_i);
    ASSERT_EQ(0x12'3400'0000_i, 0x1234_i << 24_i);

}

TEST(big_integer, right_shift) {
    ASSERT_EQ(1_i, 1_i >> 0_i);
    ASSERT_EQ(1_i, 0x1'0000'0000_i >> 32_i);
    ASSERT_EQ(1_i, 0x1'0000'0000'0000'0000_i >> 64_i);
    ASSERT_EQ(1_i, 0x1'0000'0000'0000'0000'0000'0000_i >> 96_i);

    ASSERT_EQ(1_i, 0b10_i >> 1_i);
    ASSERT_EQ(1_i, 0x2'0000'0000_i >> 33_i);
    ASSERT_EQ(0x1234_i, 0x12'3400'0000_i >> 24_i);
}



TEST(big_integer, div_32_bit) {
    ASSERT_EQ(1_i / 1_i, 1_i);
    ASSERT_EQ(2_i / 2_i, 1_i);
    ASSERT_EQ(10_i / 10_i, 1_i);

    ASSERT_EQ(2_i / 1_i, 2_i);
    ASSERT_EQ(3_i / 1_i, 3_i);
    ASSERT_EQ(30_i / 10_i, 3_i);

    ASSERT_EQ(10_i / 2_i, 5_i);
    ASSERT_EQ(27_i / 5_i, 5_i);
    ASSERT_EQ(42_i / 1_i, 42_i);
    ASSERT_EQ(17_i / 17_i, 1_i);
    ASSERT_EQ(23_i / 47_i, 0_i);
    ASSERT_EQ(4'294'967'295_i / 2_i, 2'147'483'647_i);
    ASSERT_EQ(4'294'967'295_i / 1_i, 4'294'967'295_i);
}

TEST(big_integer, mod_32_bit) {
    ASSERT_EQ(1_i % 1_i, 0_i);
    ASSERT_EQ(2_i % 2_i, 0_i);
    ASSERT_EQ(10_i % 10_i, 0_i);

    ASSERT_EQ(2_i % 1_i, 0_i);
    ASSERT_EQ(3_i % 1_i, 0_i);
    ASSERT_EQ(30_i % 10_i, 0_i);

    ASSERT_EQ(10_i % 2_i, 0_i);
    ASSERT_EQ(27_i % 5_i, 2_i);
    ASSERT_EQ(42_i % 1_i, 0_i);
    ASSERT_EQ(17_i % 17_i, 0_i);
    ASSERT_EQ(23_i % 47_i, 23_i);
    ASSERT_EQ(4'294'967'295_i % 2_i, 1_i);
    ASSERT_EQ(4'294'967'295_i % 1_i, 0_i);
}


TEST(big_integer, div_64_bit) {
    ASSERT_EQ(18'446'744'073'709'551'615_i / 2_i, 9'223'372'036'854'775'807_i);
    ASSERT_EQ(18'446'744'073'709'551'615_i / 1_i, 18'446'744'073'709'551'615_i);
    ASSERT_EQ(18'446'744'073'709'551'614_i / 42_i, 439'208'192'231'179'800_i);
    ASSERT_EQ(18'446'744'073'709'551'614_i / 32'767_i, 562'967'133'814'800_i);
}


TEST(big_integer, mod_64_bit) {
    ASSERT_EQ(18'446'744'073'709'551'615_i % 2_i, 1_i);
    ASSERT_EQ(18'446'744'073'709'551'615_i % 1_i, 0_i);
    ASSERT_EQ(18'446'744'073'709'551'614_i % 42_i, 14_i);
    ASSERT_EQ(18'446'744'073'709'551'614_i % 32'767_i, 14_i);
}

TEST(big_integer, divmod_keyboardcat) {
    auto const result = safe::detail::unsigned_divmod(
        89523478923458679234578963243425435789234587902345689745308943567897897655435234567567089_i,
        45353414312430989879875678678967584568765_i);

    ASSERT_EQ(result.quotient, 1973908255434719182486486204131416129221088053654_i);
    ASSERT_EQ(result.remainder, 16400436199443006705417866284335795049779_i);
}


TEST(big_integer, signed_compare) {
    ASSERT_TRUE(1_i > -1_i);
    ASSERT_TRUE(-1_i < 1_i);

    ASSERT_TRUE(1_i > 0_i);
    ASSERT_TRUE(0_i < 1_i);

    ASSERT_TRUE(-1_i < 0_i);
    ASSERT_TRUE(0_i > -1_i);

    ASSERT_TRUE(10_i > 5_i);
    ASSERT_TRUE(5_i < 10_i);

    ASSERT_TRUE(-10_i < -5_i);
    ASSERT_TRUE(-5_i > -10_i);
}

TEST(big_integer, signed_mul) {
    ASSERT_EQ(1_i * 1_i, 1_i);
    ASSERT_EQ(-1_i * 1_i, -1_i);
    ASSERT_EQ(1_i * -1_i, -1_i);
    ASSERT_EQ(-1_i * -1_i, 1_i);
}

TEST(big_integer, signed_div) {
    ASSERT_EQ(1_i / 1_i, 1_i);
    ASSERT_EQ(-1_i / 1_i, -1_i);
    ASSERT_EQ(1_i / -1_i, -1_i);
    ASSERT_EQ(-1_i / -1_i, 1_i);
}

TEST(big_integer, signed_mod) {
    ASSERT_EQ(1_i % 1_i, 0_i);
    ASSERT_EQ(-1_i % 1_i, 0_i);
    ASSERT_EQ(1_i % -1_i, 0_i);
    ASSERT_EQ(-1_i % -1_i, 0_i);
}