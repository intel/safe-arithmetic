#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;


using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

using safe::checked;


TEST(safe_checked_test, normal_addition) {
    EXPECT_TRUE((checked{5} + checked{10} == checked{15}).value());
    EXPECT_TRUE((checked{1500000000} + checked{1500000000u} == checked{3000000000u}).value());

    EXPECT_TRUE((checked{-1} + checked{0} == checked{-1}).value());
    EXPECT_TRUE((checked{1} + checked{0} == checked{1}).value());
}

TEST(safe_checked_test, overflow_addition) {
    EXPECT_TRUE((checked{1500000000} + checked{1500000000}).is_overflow());
}

TEST(safe_checked_test, underflow_addition) {
    EXPECT_TRUE((checked{-1500000000} + checked{-1500000000}).is_overflow());
}

TEST(safe_checked_test, overflow_plus_overflow) {
    checked const a{0, true};
    checked const b{0, true};
    checked const c{0, false};

    EXPECT_TRUE((a + b).is_overflow());
    EXPECT_TRUE((a + c).is_overflow());
    EXPECT_TRUE((c + b).is_overflow());
    EXPECT_FALSE((c + c).is_overflow());
}


TEST(safe_checked_test, addition_edges) {
    EXPECT_TRUE((checked{0} + checked{INT_MAX} == checked{INT_MAX}).value());
    EXPECT_TRUE((checked{1} + checked{INT_MAX}).is_overflow());

    EXPECT_TRUE((checked{0} + checked{INT_MIN} == checked{INT_MIN}).value());
    EXPECT_TRUE((checked{-1} + checked{INT_MIN}).is_overflow());


}

TEST(safe_checked_test, normal_subtraction) {
    EXPECT_TRUE((checked{5} - checked{10} == checked{-5}).value());
    EXPECT_TRUE((checked{1500000000u} - checked{1500000000u} == checked{0u}).value());

    EXPECT_TRUE((checked{-1} - checked{0} == checked{-1}).value());
    EXPECT_TRUE((checked{1} - checked{0} == checked{1}).value());
}

TEST(safe_checked_test, overflow_subtraction) {
    EXPECT_TRUE((checked{1500000000} - checked{-1500000000}).is_overflow());
}

TEST(safe_checked_test, underflow_subtraction) {
    EXPECT_TRUE((checked{-1500000000} - checked{1500000000}).is_overflow());
}

TEST(safe_checked_test, overflow_minus_overflow) {
    checked const a{0, true};
    checked const b{0, true};
    checked const c{0, false};

    EXPECT_TRUE((a - b).is_overflow());
    EXPECT_TRUE((a - c).is_overflow());
    EXPECT_TRUE((c - b).is_overflow());
    EXPECT_FALSE((c - c).is_overflow());
}



TEST(safe_checked_test, normal_multiplication) {
    EXPECT_TRUE((checked{5} * checked{10} == checked{50}).value());
    EXPECT_TRUE((checked{-1} * checked{0} == checked{0}).value());
    EXPECT_TRUE((checked{1} * checked{18} == checked{18}).value());
}

TEST(safe_checked_test, overflow_multiplication) {
    EXPECT_TRUE((checked{1500000000} * checked{1500000000}).is_overflow());

    EXPECT_TRUE((checked{-1} * checked{std::numeric_limits<int>::lowest()}).is_overflow());
    EXPECT_TRUE((checked{std::numeric_limits<int>::lowest()} * checked{-1}).is_overflow());
}

TEST(safe_checked_test, underflow_multiplication) {
    EXPECT_TRUE((checked{-1500000000} * checked{1500000000}).is_overflow());
}

TEST(safe_checked_test, overflow_times_overflow) {
    checked const a{0, true};
    checked const b{0, true};
    checked const c{0, false};

    EXPECT_TRUE((a * b).is_overflow());
    EXPECT_TRUE((a * c).is_overflow());
    EXPECT_TRUE((c * b).is_overflow());
    EXPECT_FALSE((c * c).is_overflow());
}

TEST(safe_checked_test, normal_division) {
    EXPECT_TRUE((checked{12} / checked{6} == checked{2}).value());
    EXPECT_TRUE((checked{-1000} / checked{10} == checked{-100}).value());
    EXPECT_TRUE((checked{84} / checked{2} == checked{42}).value());
}

TEST(safe_checked_test, overflow_division) {
    EXPECT_TRUE((checked{-1} / checked{std::numeric_limits<int>::lowest()}).is_overflow());
    EXPECT_TRUE((checked{std::numeric_limits<int>::lowest()} / checked{-1}).is_overflow());
}

TEST(safe_checked_test, divide_by_zero) {
    checked const a{1, false};
    checked const b{0, false};

    EXPECT_TRUE((a / b).is_overflow());
}

TEST(safe_checked_test, overflow_divided_by_overflow) {
    checked const a{1, true};
    checked const b{1, true};
    checked const c{1, false};

    EXPECT_TRUE((a / b).is_overflow());
    EXPECT_TRUE((a / c).is_overflow());
    EXPECT_TRUE((c / b).is_overflow());
    EXPECT_FALSE((c / c).is_overflow());
}

TEST(safe_checked_test, normal_shift_left) {
    EXPECT_TRUE(((checked{1u} << checked{31}) == checked{0x80000000u}).value());
    EXPECT_TRUE(((checked{0u} << checked{32}) == checked{0u}).value());
    EXPECT_TRUE(((checked{1u} << checked{15}) == checked{0x8000u}).value());
    EXPECT_TRUE(((checked{1} << checked{15}) == checked{0x8000}).value());
}

TEST(safe_checked_test, overflow_shift_left) {
    EXPECT_TRUE((checked{1} << checked{31}).is_overflow());
    EXPECT_TRUE((checked{1u} << checked{32}).is_overflow());
    EXPECT_TRUE((checked{0x80u} << checked{25}).is_overflow());
    EXPECT_TRUE((checked{-256} << checked{23}).is_overflow());
}

TEST(safe_checked_test, negative_shift_left) {
    EXPECT_TRUE((checked{256u} << checked{-1}).is_overflow());
}

TEST(safe_checked_test, overflow_shift_left_overflow) {
    checked const a{1, true};
    checked const b{1, true};
    checked const c{1, false};

    EXPECT_TRUE((a << b).is_overflow());
    EXPECT_TRUE((a << c).is_overflow());
    EXPECT_TRUE((c << b).is_overflow());
    EXPECT_FALSE((c << c).is_overflow());
}

TEST(safe_checked_test, normal_shift_right) {
    EXPECT_TRUE(((checked{0x80000000u} >> checked{31}) == checked{1u}).value());
    EXPECT_TRUE(((checked{-16} >> checked{1}) == checked{-8}).value());
    EXPECT_TRUE(((checked{-2} >> checked{1}) == checked{-1}).value());
    EXPECT_TRUE(((checked{-1} >> checked{1}) == checked{-1}).value());
}

TEST(safe_checked_test, overflow_shift_right_overflow) {
    checked const a{1, true};
    checked const b{1, true};
    checked const c{1, false};

    EXPECT_TRUE((a >> b).is_overflow());
    EXPECT_TRUE((a >> c).is_overflow());
    EXPECT_TRUE((c >> b).is_overflow());
    EXPECT_FALSE((c >> c).is_overflow());
}