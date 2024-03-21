#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe/big_integer_gen.hpp>

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;
using namespace boost::mp11;

template <typename T> struct test_interval {
    T min;
    T max;
};

template <typename T, auto lhs_min, auto lhs_max, auto rhs_min, auto rhs_max,
          typename OperationT>
void check_interval(constrain_interval_t<lhs_min, lhs_max> lhs, constrain_interval_t<rhs_min, rhs_max> rhs,
                    OperationT op) {
    auto const actual = safe::dsl::detail::simp(op(lhs, rhs));
    for (T i = lhs_min; i <= lhs_max; i = constraint_cast<T>(i + 1)) {
        for (T j = rhs_min; j <= rhs_max; j = constraint_cast<T>(j + 1)) {
            auto const v = op(i, j);
            EXPECT_GE(v, actual.min);
            EXPECT_LE(v, actual.max);
        }
    }
}

template <typename... Tn> void run_tests(mp_list<Tn...>) { (Tn::run(), ...); }

template <template <typename, typename> typename OperationT, typename LhsListT,
          typename RhsListT>
constexpr void test_operation() {
    run_tests(mp_product<OperationT, LhsListT, RhsListT>{});
}

template <typename LhsT, typename RhsT> struct modulo_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr bool rhs_nonzero =
            (rhs.min < 0 && rhs.max < 0) || (rhs.min > 0 && rhs.max > 0);

        if constexpr (rhs_nonzero) {
            check_interval<safe::big_integer<96>>(
                lhs, rhs, [](auto a, auto b) { return a % b; });
        }
    }
};

template <typename LhsT, typename RhsT> struct divide_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr bool rhs_nonzero =
            (rhs.min < 0 && rhs.max < 0) || (rhs.min > 0 && rhs.max > 0);

        if constexpr (rhs_nonzero) {
            check_interval<safe::big_integer<96>>(
                lhs, rhs, [](auto a, auto b) { return a / b; });
        }
    }
};

template <typename LhsT, typename RhsT> struct add_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        check_interval<safe::big_integer<96>>(
            lhs, rhs, [](auto a, auto b) { return a + b; });
    }
};

template <typename LhsT, typename RhsT> struct minus_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        check_interval<safe::big_integer<96>>(
            lhs, rhs, [](auto a, auto b) { return a - b; });
    }
};

template <typename LhsT, typename RhsT> struct multiply_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        check_interval<safe::big_integer<96>>(
            lhs, rhs, [](auto a, auto b) { return a * b; });
    }
};

template <auto scale>
using operands =
    mp_list<constrain_interval_t<-5 * scale, -3 * scale>, constrain_interval_t<-4 * scale, -2 * scale>,
            constrain_interval_t<-3 * scale, -1 * scale>, constrain_interval_t<-2 * scale, 0 * scale>,
            constrain_interval_t<-1 * scale, 1 * scale>, constrain_interval_t<0 * scale, 2 * scale>,
            constrain_interval_t<1 * scale, 3 * scale>, constrain_interval_t<2 * scale, 4 * scale>,
            constrain_interval_t<3 * scale, 5 * scale>>;

TEST(safe_dsl_test, add_op) {
    test_operation<add_test_op, operands<5>, operands<5>>();
}

TEST(safe_dsl_test, minus_op) {
    test_operation<minus_test_op, operands<1>, operands<1>>();
}

TEST(safe_dsl_test, multiply_op) {
    test_operation<multiply_test_op, operands<5>, operands<5>>();
}

TEST(safe_dsl_test, divide_op) {
    test_operation<divide_test_op, operands<5>, operands<5>>();
}

// template<typename T>
// struct show {
//     constexpr static bool value = false;
// };

TEST(safe_dsl_test, modulo_op) {
    EXPECT_EQ((constrain_interval<101, 1000> % constrain_interval<1, 100>), (constrain_interval<0, 99>));

    EXPECT_EQ((constrain_interval<100, 1000> % constrain_interval<100, 1000>),
              (constrain_interval<0, 999> || constrain_interval<100, 999>));

    EXPECT_EQ((constrain_interval<100, 1000> % constrain_interval<2000, 3000>), (constrain_interval<100, 1000>));

    EXPECT_EQ((constrain_interval<-1000, -100> % constrain_interval<2000, 3000>), (constrain_interval<-1000, -100>));

    EXPECT_EQ((constrain_interval<-1000, -100> % constrain_interval<100, 1000>),
              (constrain_interval<-900, 0> || constrain_interval<-999, -100>));

    EXPECT_EQ((constrain_interval<-1000, -101> % constrain_interval<1, 100>), (constrain_interval<-99, 0>));
    //    test_operation<
    //        modulo_test_op,
    //        operands<5>,
    //        operands<5>
    //    >();

    //    test_operation<
    //        modulo_test_op,
    //        operands<5>,
    //        operands<10>
    //    >();

    //    test_operation<
    //        modulo_test_op,
    //        operands<10>,
    //        operands<5>
    //    >();

    //    EXPECT_EQ(
    //        (constrain_interval<0, 1000> % constrain_interval<100, 100>),
    //        (constrain_interval<0l, 99l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<0, 50> % constrain_interval<100, 100>),
    //        (constrain_interval<0l, 50l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<0, 300> % constrain_interval<1000, 1000>),
    //        (constrain_interval<0l, 300l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<0, 100> % constrain_interval<1, 100>),
    //        (constrain_interval<0l, 99l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<-100, 1> % constrain_interval<1, 100>),
    //        (constrain_interval<-99l, 1l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<-100, 100> % constrain_interval<1, 100>),
    //        (constrain_interval<-99l, 99l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<-100, 100> % constrain_interval<1, 1000>),
    //        (constrain_interval<-100l, 100l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<0, 50> % constrain_interval<1, 100>),
    //        (constrain_interval<0l, 50l>)
    //    );

    //    EXPECT_EQ(
    //        (constrain_interval<0, 100> % constrain_interval<1, 50>),
    //        (constrain_interval<0l, 49l>)
    //    );
}

TEST(safe_dsl_test, ival_union_simplification) {
    EXPECT_EQ((constrain_interval<20, 30> || constrain_interval<0, 10>), (constrain_interval<0, 10> || constrain_interval<20, 30>));

    EXPECT_EQ((constrain_interval<0, 10> || constrain_interval<10, 30>), (constrain_interval<0, 30>));

    EXPECT_EQ((constrain_interval<0, 10> || constrain_interval<11, 30>), (constrain_interval<0, 30>));

    EXPECT_NE((constrain_interval<0, 10> || constrain_interval<12, 30>), (constrain_interval<0, 30>));
}

TEST(safe_dsl_test, set_union_simplification) {
    EXPECT_EQ((constrain_set<0, 1, 2, 3, 4>), (constrain_interval<0, 4>));

    EXPECT_EQ((constrain_set<0, 1, 2, 4, 5, 6>), (constrain_interval<0, 2> || constrain_interval<4, 6>));

    EXPECT_EQ((constrain_set<0, 1, 2, 4, 5, 6> || constrain_interval<1, 7>), (constrain_interval<0, 7>));
}

TEST(safe_dsl_test, subtraction_union_simplification) {
    EXPECT_EQ((constrain_interval<0, 10> - constrain_interval<20, 30>), (constrain_interval<-30, -10>));

    EXPECT_EQ(((constrain_interval<0, 10> || constrain_interval<40, 80>)-constrain_interval<20, 30>),
              (constrain_interval<-30, -10> || constrain_interval<10, 60>));

    EXPECT_EQ((constrain_interval<20, 30> - (constrain_interval<0, 10> || constrain_interval<40, 80>)),
              (constrain_interval<10, 30> || constrain_interval<-60, -10>));

    EXPECT_EQ(
        ((constrain_interval<0, 10> || constrain_interval<100, 200>)-(constrain_interval<20, 30> || constrain_interval<400, 800>)),
        (constrain_interval<-800, -200> || constrain_interval<-30, -10> || constrain_interval<70, 180>));
}

TEST(safe_dsl_test, multiplication_union_simplification) {
    EXPECT_EQ((constrain_interval<0, 10> * constrain_interval<20, 30>), (constrain_interval<0, 300>));

    EXPECT_EQ(((constrain_interval<0, 10> || constrain_interval<40, 80>)*constrain_interval<20, 30>),
              (constrain_interval<0, 300> || constrain_interval<800, 2400>));

    EXPECT_EQ((constrain_interval<20, 30> * (constrain_interval<0, 10> || constrain_interval<40, 80>)),
              (constrain_interval<0, 300> || constrain_interval<800, 2400>));

    EXPECT_EQ(
        ((constrain_interval<0, 10> || constrain_interval<100, 200>)*(constrain_interval<20, 30> || constrain_interval<400, 800>)),
        (constrain_interval<0, 8000> || constrain_interval<40000, 160000>));
}

TEST(safe_dsl_test, shift_left_op) {
    EXPECT_EQ((constrain_interval<0, 10> << constrain_interval<0, 10>), (constrain_interval<0, 10240>));
}

TEST(safe_dsl_test, complicated_expression_simplification) {
    EXPECT_EQ(((constrain_interval<0, 10> || (constrain_interval<100, 200> + constrain_set<2, 7, 9, 42>) ||
                constrain_interval<300, 600>)*(constrain_interval<20, 30> || constrain_interval<400, 800>)),
              (constrain_interval<0, 18000> || constrain_interval<40800, 480000>));
}

TEST(safe_dsl_test, mask_shift_left) {
    EXPECT_EQ((constrain_mask<0b0> << constrain_interval<0, 0>), constrain_mask<0b0>);
    EXPECT_EQ((constrain_mask<0b1> << constrain_interval<1, 1>), constrain_mask<0b10>);

    EXPECT_EQ((constrain_mask<0b1> << constrain_interval<0, 1>), constrain_mask<0b11>);
    EXPECT_EQ((constrain_mask<0b1> << constrain_interval<0, 4>), constrain_mask<0b11111>);
    EXPECT_EQ((constrain_mask<0b1> << constrain_interval<4, 8>), constrain_mask<0b111110000>);

    EXPECT_EQ((constrain_mask<0b1000001> << constrain_interval<0, 1>), constrain_mask<0b11000011>);
    EXPECT_EQ((constrain_mask<0b1000001> << constrain_interval<2, 4>), constrain_mask<0b11100011100>);
}

TEST(safe_dsl_test, mask_shift_right) {
    EXPECT_EQ((constrain_mask<0b1000> >> constrain_interval<0, 0>), constrain_mask<0b1000>);
    EXPECT_EQ((constrain_mask<0b1000> >> constrain_interval<1, 1>), constrain_mask<0b0100>);

    EXPECT_EQ((constrain_mask<0b1000> >> constrain_interval<0, 1>), constrain_mask<0b1100>);
    EXPECT_EQ((constrain_mask<0b10000000> >> constrain_interval<4, 6>), constrain_mask<0b1110>);
}
