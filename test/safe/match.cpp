#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

using safe::constrain_interval;

struct mock_function {
    MOCK_METHOD(void, two_safe_vars, (int, int));
};

mock_function *mock_function_ptr{};

class safe_match_test : public ::testing::Test {
  protected:
    mock_function my_mock_function;

    void SetUp() override { mock_function_ptr = &my_mock_function; }
};

void two_safe_vars(ival_s32<0, 10> a, ival_s32<0, 10> b) {
    mock_function_ptr->two_safe_vars(a.raw_value(), b.raw_value());
}

TEST_F(safe_match_test, simple_pass) {
    EXPECT_CALL(my_mock_function, two_safe_vars(2, 5)).Times(1);

    safe::match(two_safe_vars, []() {})(2, 5);
}

TEST_F(safe_match_test, pass_with_an_input_var) {
    EXPECT_CALL(my_mock_function, two_safe_vars(9, 3)).Times(1);

    safe::match(two_safe_vars, []() {})(9, 3_cn);
}

TEST_F(safe_match_test, pass_with_both_input_vars) {
    EXPECT_CALL(my_mock_function, two_safe_vars(0, 4)).Times(1);

    safe::match(two_safe_vars, []() {})(0_cn, 4_cn);
}

TEST_F(safe_match_test, simple_fail) {
    EXPECT_CALL(my_mock_function, two_safe_vars(_, _)).Times(0);
    bool fail = false;
    safe::match(two_safe_vars, [&]() { fail = true; })(12, 5);
    EXPECT_TRUE(fail);
}

TEST_F(safe_match_test, fail_with_an_input_var) {
    EXPECT_CALL(my_mock_function, two_safe_vars(_, _)).Times(0);
    bool fail = false;
    safe::match(two_safe_vars, [&]() { fail = true; })(11, 9_cn);
    EXPECT_TRUE(fail);
}

TEST_F(safe_match_test, char_to_ord_example) {
    auto const char_to_ord = safe::match(
        [](ival_s32<'0', '9'> dec_digit) { return dec_digit - s32_<'0'>; },
        [](ival_s32<'a', 'f'> lower_hex_digit) {
            return lower_hex_digit - s32_<'a'> + 10_cn;
        },
        [](ival_s32<'A', 'F'> upper_hex_digit) {
            return upper_hex_digit - s32_<'A'> + 10_cn;
        },
        []() { return 0_cn; });

    EXPECT_EQ(char_to_ord('0'), 0_cn);
    EXPECT_EQ(char_to_ord('3'), 3_cn);
    EXPECT_EQ(char_to_ord('9'), 9_cn);
    EXPECT_EQ(char_to_ord('a'), 10_cn);
    EXPECT_EQ(char_to_ord('A'), 10_cn);
    EXPECT_EQ(char_to_ord('f'), 15_cn);
    EXPECT_EQ(char_to_ord('F'), 15_cn);
    EXPECT_EQ(char_to_ord('g'), 0_cn);
}

TEST(safe_runtime_check_test, single_interval) {
    EXPECT_TRUE((safe::detail::check<ival_s32<0, 100>>(10)));
    EXPECT_TRUE((safe::detail::check<ival_s32<0, 100>>(0)));
    EXPECT_TRUE((safe::detail::check<ival_s32<0, 100>>(100)));

    EXPECT_FALSE((safe::detail::check<ival_s32<0, 100>>(-10)));
    EXPECT_FALSE((safe::detail::check<ival_s32<0, 100>>(-1)));
    EXPECT_FALSE((safe::detail::check<ival_s32<0, 100>>(101)));
}

TEST(safe_runtime_check_test, single_interval_var) {
    EXPECT_TRUE((safe::detail::check<ival_s32<0, 100>>(10_cn)));
    EXPECT_TRUE((safe::detail::check<ival_s32<0, 100>>(0_cn)));
    EXPECT_TRUE((safe::detail::check<ival_s32<0, 100>>(100_cn)));

    EXPECT_FALSE((safe::detail::check<ival_s32<0, 100>>(-10_cn)));
    EXPECT_FALSE((safe::detail::check<ival_s32<0, 100>>(-1_cn)));
    EXPECT_FALSE((safe::detail::check<ival_s32<0, 100>>(101_cn)));
}

TEST(safe_runtime_check_test, interval_union) {
    using safe_var = safe::constrained_number<constrain_interval<0, 10> || constrain_interval<20, 30>, int>;

    EXPECT_TRUE((safe::detail::check<safe_var>(0)));
    EXPECT_TRUE((safe::detail::check<safe_var>(5)));
    EXPECT_TRUE((safe::detail::check<safe_var>(10)));
    EXPECT_TRUE((safe::detail::check<safe_var>(20)));
    EXPECT_TRUE((safe::detail::check<safe_var>(25)));
    EXPECT_TRUE((safe::detail::check<safe_var>(30)));

    EXPECT_FALSE((safe::detail::check<safe_var>(-1)));
    EXPECT_FALSE((safe::detail::check<safe_var>(11)));
    EXPECT_FALSE((safe::detail::check<safe_var>(15)));
    EXPECT_FALSE((safe::detail::check<safe_var>(19)));
    EXPECT_FALSE((safe::detail::check<safe_var>(31)));
    EXPECT_FALSE((safe::detail::check<safe_var>(1000)));
}
