#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

#include <type_traits>

using namespace safe::literals;
using namespace safe;

TEST(make_constrained_test, make_from_cn_udl) {
    auto from = 10_cn;
    constexpr auto result = 
        make_constrained<constrain_interval<0, 1000>>(10_cn);

    using result_t = std::remove_cvref_t<decltype(result)>;

    EXPECT_EQ(result, from);
    EXPECT_TRUE((std::is_same_v<decltype(from), result_t>));
}

TEST(make_constrained_test, make_from_integral_constant) {
    constexpr auto result = 
        make_constrained<constrain_interval<0, 1000>>(std::integral_constant<int, 10>{});

    EXPECT_EQ(result, 10);
}

TEST(make_constrained_test, make_with_runtime_check) {
    auto result = 
        make_constrained<constrain_interval<0, 1000>>(10);

    EXPECT_EQ(result, 10);
}

TEST(make_constrained_test, make_with_failing_runtime_check) {
    EXPECT_THAT([]() { (make_constrained<constrain_interval<0, 1000>>(-10)); },
        ThrowsMessage<std::range_error>(testing::HasSubstr("make_constrained constraint violation error")));
}