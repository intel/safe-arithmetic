#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;


using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

using safe::ival;

TEST(safe_runtime_check_test, single_interval) {
    EXPECT_TRUE((safe::check<ival_s32<0, 100>>(10)));
    EXPECT_TRUE((safe::check<ival_s32<0, 100>>(0)));
    EXPECT_TRUE((safe::check<ival_s32<0, 100>>(100)));

    EXPECT_FALSE((safe::check<ival_s32<0, 100>>(-10)));
    EXPECT_FALSE((safe::check<ival_s32<0, 100>>(-1)));
    EXPECT_FALSE((safe::check<ival_s32<0, 100>>(101)));
}

TEST(safe_runtime_check_test, single_interval_var) {
    EXPECT_TRUE((safe::check<ival_s32<0, 100>>(10_s32)));
    EXPECT_TRUE((safe::check<ival_s32<0, 100>>(0_s32)));
    EXPECT_TRUE((safe::check<ival_s32<0, 100>>(100_s32)));

    EXPECT_FALSE((safe::check<ival_s32<0, 100>>(-10_s32)));
    EXPECT_FALSE((safe::check<ival_s32<0, 100>>(-1_s32)));
    EXPECT_FALSE((safe::check<ival_s32<0, 100>>(101_s32)));
}

TEST(safe_runtime_check_test, interval_union) {
    using safe_var = safe::var<int, ival<0, 10> || ival<20, 30>>;

    EXPECT_TRUE((safe::check<safe_var>(0)));
    EXPECT_TRUE((safe::check<safe_var>(5)));
    EXPECT_TRUE((safe::check<safe_var>(10)));
    EXPECT_TRUE((safe::check<safe_var>(20)));
    EXPECT_TRUE((safe::check<safe_var>(25)));
    EXPECT_TRUE((safe::check<safe_var>(30)));

    EXPECT_FALSE((safe::check<safe_var>(-1)));
    EXPECT_FALSE((safe::check<safe_var>(11)));
    EXPECT_FALSE((safe::check<safe_var>(15)));
    EXPECT_FALSE((safe::check<safe_var>(19)));
    EXPECT_FALSE((safe::check<safe_var>(31)));
    EXPECT_FALSE((safe::check<safe_var>(1000)));
}