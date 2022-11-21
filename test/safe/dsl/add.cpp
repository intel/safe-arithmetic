#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;


TEST(safe_dsl_add, add_two_ival_constants) {
    EXPECT_EQ(
        (ival<30, 30> + ival<12, 12>),
        (ival<42, 42>)
    );
}

TEST(safe_dsl_add, add_two_intervals) {
    EXPECT_EQ(
        (ival<10, 20> + ival<40, 80>),
        (ival<50, 100>)
    );
}

TEST(safe_dsl_add, add_two_mask_constants) {
    EXPECT_EQ(
        (mask<0, 12> + mask<0, 8>),
        (ival<20, 20>)
    );
}

TEST(safe_dsl_add, add_two_masks_1) {
    EXPECT_EQ(
        (mask<0xff> + mask<0xff>),
        (ival<0, 255 + 255>)
    );
}

TEST(safe_dsl_add, add_two_masks_2) {
    EXPECT_EQ(
        (mask<0xf> + mask<0xff>),
        (ival<0, 15 + 255>)
    );
}

TEST(safe_dsl_add, union_simplification) {
    EXPECT_EQ(
        ((ival<0, 10> || ival<40, 80>) + ival<20, 30>),
        (ival<20, 40> || ival<60, 110>)
    );

    EXPECT_EQ(
        (ival<20, 30> + (ival<0, 10> || ival<40, 80>)),
        (ival<20, 40> || ival<60, 110>)
    );

    EXPECT_EQ(
        ((ival<0, 10> || ival<100, 200>) + (ival<20, 30> || ival<400, 800>)),
        (ival<20, 40> || ival<120, 230> || ival<400, 1000>)
    );
}
