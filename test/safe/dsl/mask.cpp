#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_mask, create_mask) {
    [[maybe_unused]] auto v = mask<0b1001>;
}

TEST(safe_dsl_mask, mask_subset) {
    EXPECT_TRUE(mask<0b1111> >= mask<0b1010>);
    EXPECT_TRUE(mask<0b1111> >= mask<0b0000>);
    EXPECT_TRUE(mask<0b1111> >= mask<0b1111>);

    EXPECT_FALSE(mask<0b1001> >= mask<0b1010>);
    EXPECT_FALSE(mask<0b0001> >= mask<0b0010>);
    EXPECT_FALSE(mask<0b0001> >= mask<0b0011>);
}

TEST(safe_dsl_mask, mask_superset) {
    EXPECT_TRUE(mask<0b1000> <= mask<0b1100>);
    EXPECT_TRUE(mask<0b0000> <= mask<0b0000>);
    EXPECT_TRUE(mask<0b0001> <= mask<0b1011>);

    EXPECT_FALSE(mask<0b1001> <= mask<0b1100>);
    EXPECT_FALSE(mask<0b0010> <= mask<0b0000>);
    EXPECT_FALSE(mask<0b0100> <= mask<0b1011>);
}

TEST(safe_dsl_mask, mask_equal) {
    EXPECT_TRUE(mask<0b1000> == mask<0b1000>);
    EXPECT_TRUE(mask<0b0000> == mask<0b0000>);
    EXPECT_TRUE(mask<0b0001> == mask<0b0001>);

    EXPECT_FALSE(mask<0b1000> == mask<0b1001>);
    EXPECT_FALSE(mask<0b0011> == mask<0b0010>);
    EXPECT_FALSE(mask<0b0110> == mask<0b0100>);
}

TEST(safe_dsl_mask, mask_not_equal) {
    EXPECT_FALSE(mask<0b1000> != mask<0b1000>);
    EXPECT_FALSE(mask<0b0000> != mask<0b0000>);
    EXPECT_FALSE(mask<0b0001> != mask<0b0001>);

    EXPECT_TRUE(mask<0b1000> != mask<0b1001>);
    EXPECT_TRUE(mask<0b0011> != mask<0b0010>);
    EXPECT_TRUE(mask<0b0110> != mask<0b0100>);
}


TEST(safe_dsl_mask, detail_is_basic_mask) {
    EXPECT_TRUE(dsl::detail::is_basic_mask(0xffffffff));
    EXPECT_TRUE(dsl::detail::is_basic_mask(0xffffffffu));

    EXPECT_TRUE(dsl::detail::is_basic_mask(0b1111111111));
    EXPECT_TRUE(dsl::detail::is_basic_mask(0b0000111111));
    EXPECT_TRUE(dsl::detail::is_basic_mask(0b0000000001));
    EXPECT_TRUE(dsl::detail::is_basic_mask(0b0000000000));


    EXPECT_FALSE(dsl::detail::is_basic_mask(0x80000000));
    EXPECT_FALSE(dsl::detail::is_basic_mask(0b0000000010));
    EXPECT_FALSE(dsl::detail::is_basic_mask(0b0100011111));
    EXPECT_FALSE(dsl::detail::is_basic_mask(0b1111110111));
}

TEST(safe_dsl_test, fill_bitmask) {
    EXPECT_EQ(dsl::detail::fill_in_bitmask(0b1111), 0b1111);
    EXPECT_EQ(dsl::detail::fill_in_bitmask(0b1101), 0b1111);
    EXPECT_EQ(dsl::detail::fill_in_bitmask(0b1000), 0b1111);
}

TEST(safe_dsl_test, ival_pwr2_range_to_mask) {
    using ival_mask_255 = dsl::detail::to_mask_t<dsl::ival_t<0, 255>>;
    EXPECT_EQ(ival_mask_255::var_bits, 255);
    EXPECT_EQ(ival_mask_255::const_bits, 0);

    using ival_mask_15 = dsl::detail::to_mask_t<dsl::ival_t<0, 15>>;
    EXPECT_EQ(ival_mask_15::var_bits, 15);
    EXPECT_EQ(ival_mask_15::const_bits, 0);
}

TEST(safe_dsl_test, ival_0_12_to_mask) {
    using ival_mask = dsl::detail::to_mask_t<dsl::ival_t<0, 12>>;
    EXPECT_EQ(ival_mask::var_bits, 0b1111);
    EXPECT_EQ(ival_mask::const_bits, 0);
}

TEST(safe_dsl_test, ival_0_137_to_mask) {
    using ival_mask = dsl::detail::to_mask_t<dsl::ival_t<0, 137>>;
    EXPECT_EQ(ival_mask::var_bits, 0xff);
    EXPECT_EQ(ival_mask::const_bits, 0);
}

TEST(safe_dsl_test, ival_w_offset_to_mask_1) {
    using ival_mask = dsl::detail::to_mask_t<dsl::ival_t<512, 512 + 137>>;
    EXPECT_EQ(ival_mask::var_bits, 0xff);
    EXPECT_EQ(ival_mask::const_bits, 512);
}

TEST(safe_dsl_test, ival_w_offset_to_mask_2) {
    using ival_mask = dsl::detail::to_mask_t<dsl::ival_t<127, 128>>;
    EXPECT_EQ(ival_mask::var_bits, 0xff);
    EXPECT_EQ(ival_mask::const_bits, 0);
}

TEST(safe_dsl_test, ival_const_42_to_mask) {
    using ival_mask = dsl::detail::to_mask_t<dsl::ival_t<42, 42>>;
    EXPECT_EQ(ival_mask::var_bits, 0);
    EXPECT_EQ(ival_mask::const_bits, 42);
}

TEST(safe_dsl_test, mask_14_to_ival) {
    using mask_ival = dsl::detail::to_ival_t<mask_t<0b1110>>;
    EXPECT_EQ(mask_ival::min, 0);
    EXPECT_EQ(mask_ival::max, 14);
}

TEST(safe_dsl_test, mask_8_to_ival) {
    using mask_ival = dsl::detail::to_ival_t<mask_t<0b1000>>;
    EXPECT_EQ(mask_ival::min, 0);
    EXPECT_EQ(mask_ival::max, 8);
}

TEST(safe_dsl_test, mask_8_256_to_ival) {
    using mask_ival = dsl::detail::to_ival_t<mask_t<0b1000, 256>>;
    EXPECT_EQ(mask_ival::min, 256);
    EXPECT_EQ(mask_ival::max, 256 + 8);
}

TEST(safe_dsl_test, mask_const_to_ival) {
    using mask_ival = dsl::detail::to_ival_t<mask_t<0, 42>>;
    EXPECT_EQ(mask_ival::min, 42);
    EXPECT_EQ(mask_ival::max, 42);
}


TEST(safe_dsl_test, mask_ival_eq) {
    EXPECT_TRUE((ival<0, 15> <= mask<0b1111>));
    EXPECT_TRUE((ival<0, 15> >= mask<0b1111>));

    EXPECT_TRUE((ival<0, 15> == mask<0b1111>));

    EXPECT_TRUE((ival<0, 14> >= mask<0b1110>));
    EXPECT_FALSE((ival<0, 14> <= mask<0b1110>));
    EXPECT_TRUE((ival<0, 14> != mask<0b1110>));
}
