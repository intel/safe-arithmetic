#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;



using namespace safe;
using namespace safe::literals;

template<typename T>
struct test_interval {
    T min;
    T max;
};

template<typename T, auto lhs_min, auto lhs_max, auto rhs_min, auto rhs_max, typename OperationT>
[[nodiscard]] constexpr auto calc_interval(
    ival_t<lhs_min, lhs_max> lhs,
    ival_t<rhs_min, rhs_max> rhs,
    OperationT op
) {
//    using T = std::common_type_t<
//        decltype(lhs_min),
//        decltype(lhs_max),
//        decltype(rhs_min),
//        decltype(rhs_max)
//    >;

    constexpr std::pair<T, T> result = [](){
        std::pair<T, T> r{std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest()};

        for (T i = lhs_min; i <= lhs_max; i++) {
            for (T j = rhs_min; j <= rhs_max; j++) {
                auto const v = OperationT{}(i, j);
                r.first = std::min(r.first, v);
                r.second = std::max(r.second, v);
            }
        }

        return r;
    }();

    return ival<result.first, result.second>;
}

template<typename... Tn>
void run_tests(mp_list<Tn...>) {
    (Tn::run(), ...);
}

template<
    template<typename, typename> typename OperationT,
    typename LhsListT,
    typename RhsListT>
constexpr void test_operation() {
    run_tests(mp_product<OperationT, LhsListT, RhsListT>{});
}


template<typename LhsT, typename RhsT>
struct modulo_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr bool rhs_nonzero =
            (rhs.min < 0 && rhs.max < 0) ||
            (rhs.min > 0 && rhs.max > 0);

        if constexpr (rhs_nonzero) {
            constexpr auto expected =
                calc_interval<long>(lhs, rhs, [](auto a, auto b){return a % b;});

            constexpr auto actual =
                safe::dsl::detail::simp(lhs % rhs);

            EXPECT_TRUE(expected <= actual);
        }
    }
};

template<typename LhsT, typename RhsT>
struct divide_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr bool rhs_nonzero =
            (rhs.min < 0 && rhs.max < 0) ||
            (rhs.min > 0 && rhs.max > 0);

        if constexpr (rhs_nonzero) {
            constexpr auto expected =
                calc_interval<long>(lhs, rhs, [](auto a, auto b){return a / b;});

            constexpr auto actual =
                safe::dsl::detail::simp(lhs / rhs);

            EXPECT_EQ(expected.min, actual.min);
            EXPECT_EQ(expected.max, actual.max);
        }
    }
};

template<typename LhsT, typename RhsT>
struct add_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr auto expected =
            calc_interval<long>(lhs, rhs, [](auto a, auto b){return a + b;});

        constexpr auto actual =
            safe::dsl::detail::simp(lhs + rhs);

        EXPECT_EQ(expected.min, actual.min);
        EXPECT_EQ(expected.max, actual.max);
    }
};

template<typename LhsT, typename RhsT>
struct minus_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr auto expected =
            calc_interval<long>(lhs, rhs, [](auto a, auto b){return a - b;});

        constexpr auto actual =
            safe::dsl::detail::simp(lhs - rhs);

        EXPECT_EQ(expected.min, actual.min);
        EXPECT_EQ(expected.max, actual.max);
    }
};

template<typename LhsT, typename RhsT>
struct multiply_test_op {
    constexpr static LhsT lhs{};
    constexpr static RhsT rhs{};

    constexpr static void run() {
        constexpr auto expected =
            calc_interval<long>(lhs, rhs, [](auto a, auto b){return a * b;});

        constexpr auto actual =
            safe::dsl::detail::simp(lhs * rhs);

        EXPECT_EQ(expected.min, actual.min);
        EXPECT_EQ(expected.max, actual.max);
    }
};

template<auto scale>
using operands = mp_list<
    ival_t<-5 * scale, -3 * scale>,
    ival_t<-4 * scale, -2 * scale>,
    ival_t<-3 * scale, -1 * scale>,
    ival_t<-2 * scale,  0 * scale>,
    ival_t<-1 * scale,  1 * scale>,
    ival_t< 0 * scale,  2 * scale>,
    ival_t< 1 * scale,  3 * scale>,
    ival_t< 2 * scale,  4 * scale>,
    ival_t< 3 * scale,  5 * scale>
>;



TEST(safe_dsl_test, add_op) {
    test_operation<
        add_test_op,
        operands<5>,
        operands<5>
    >();
}

TEST(safe_dsl_test, minus_op) {
    test_operation<
        minus_test_op,
        operands<5>,
        operands<5>
    >();
}

TEST(safe_dsl_test, multiply_op) {
    test_operation<
        multiply_test_op,
        operands<5>,
        operands<5>
    >();
}

TEST(safe_dsl_test, divide_op) {
    test_operation<
        divide_test_op,
        operands<5>,
        operands<5>
    >();
}

template<typename T>
struct show {
    constexpr static bool value = false;
};

TEST(safe_dsl_test, modulo_op) {
    test_operation<
        modulo_test_op,
        operands<5>,
        operands<5>
    >();

    test_operation<
        modulo_test_op,
        operands<5>,
        operands<10>
    >();

    test_operation<
        modulo_test_op,
        operands<10>,
        operands<5>
    >();


    EXPECT_EQ(
        (ival<0, 1000> % ival<100, 100>),
        (ival<0l, 99l>)
    );


    EXPECT_EQ(
        (ival<0, 50> % ival<100, 100>),
        (ival<0l, 50l>)
    );


    EXPECT_EQ(
        (ival<0, 300> % ival<1000, 1000>),
        (ival<0l, 300l>)
    );

    EXPECT_EQ(
        (ival<0, 100> % ival<1, 100>),
        (ival<0l, 99l>)
    );

    EXPECT_EQ(
        (ival<-100, 1> % ival<1, 100>),
        (ival<-99l, 1l>)
    );

    EXPECT_EQ(
        (ival<-100, 100> % ival<1, 100>),
        (ival<-99l, 99l>)
    );

    EXPECT_EQ(
        (ival<-100, 100> % ival<1, 1000>),
        (ival<-100l, 100l>)
    );

    EXPECT_EQ(
        (ival<0, 50> % ival<1, 100>),
        (ival<0l, 50l>)
    );

    EXPECT_EQ(
        (ival<0, 100> % ival<1, 50>),
        (ival<0l, 49l>)
    );


}

TEST(safe_dsl_test, ival_union_simplification) {
    EXPECT_EQ(
        (ival<20, 30> || ival<0, 10>),
        (ival<0, 10> || ival<20, 30>)
    );

    EXPECT_EQ(
        (ival<0, 10> || ival<10, 30>),
        (ival<0, 30>)
    );

    EXPECT_EQ(
        (ival<0, 10> || ival<11, 30>),
        (ival<0, 30>)
    );

    EXPECT_NE(
        (ival<0, 10> || ival<12, 30>),
        (ival<0, 30>)
    );
}

TEST(safe_dsl_test, set_union_simplification) {
    EXPECT_EQ(
        (set<0, 1, 2, 3, 4>),
        (ival<0, 4>)
    );

    EXPECT_EQ(
        (set<0, 1, 2, 4, 5, 6>),
        (ival<0, 2> || ival<4, 6>)
    );

    EXPECT_EQ(
        (set<0, 1, 2, 4, 5, 6> || ival<1, 7>),
        (ival<0, 7>)
    );
}

TEST(safe_dsl_test, intersection_simplification) {
    EXPECT_EQ(
        (ival<0, 20> && ival<10, 30>),
        (ival<10, 20>)
    );
}

TEST(safe_dsl_test, addition_union_simplification) {
    EXPECT_EQ(
        (ival<0, 10> + ival<20, 30>),
        (ival<20, 40>)
    );

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

TEST(safe_dsl_test, subtraction_union_simplification) {
    EXPECT_EQ(
        (ival<0, 10> - ival<20, 30>),
        (ival<-30, -10>)
    );

    EXPECT_EQ(
        ((ival<0, 10> || ival<40, 80>) - ival<20, 30>),
        (ival<-30, -10> || ival<10, 60>)
    );

    EXPECT_EQ(
        (ival<20, 30> - (ival<0, 10> || ival<40, 80>)),
        (ival<10, 30> || ival<-60, -10>)
    );

    EXPECT_EQ(
        ((ival<0, 10> || ival<100, 200>) - (ival<20, 30> || ival<400, 800>)),
        (ival<-800, -200> || ival<-30, -10> || ival<70, 180>)
    );
}

TEST(safe_dsl_test, multiplication_union_simplification) {
    EXPECT_EQ(
        (ival<0, 10> * ival<20, 30>),
        (ival<0, 300>)
    );

    EXPECT_EQ(
        ((ival<0, 10> || ival<40, 80>) * ival<20, 30>),
        (ival<0, 300> || ival<800, 2400>)
    );

    EXPECT_EQ(
        (ival<20, 30> * (ival<0, 10> || ival<40, 80>)),
        (ival<0, 300> || ival<800, 2400>)
    );

    EXPECT_EQ(
        ((ival<0, 10> || ival<100, 200>) * (ival<20, 30> || ival<400, 800>)),
        (ival<0, 8000> || ival<40000, 160000>)
    );
}

TEST(safe_dsl_test, shift_left_op) {
    EXPECT_EQ(
        (ival<0, 10> << ival<0, 10>),
        (ival<0, 10240>)
    );
}

TEST(safe_dsl_test, superset_op) {
    EXPECT_TRUE((ival<0, 100> >= ival<10, 90>));
    EXPECT_TRUE((ival<0, 100> >= ival<0, 100>));
}

TEST(safe_dsl_test, superset_union_simplification) {
    EXPECT_TRUE((ival<0, 100> >= (ival<10, 20> || ival<30, 40>)));
    EXPECT_TRUE(((ival<0, 100> || ival<200, 300>) >= ival<10, 20>));
    EXPECT_TRUE(((ival<0, 100> || ival<200, 300>) >= ival<250, 300>));
    EXPECT_TRUE(((ival<0, 100> || ival<200, 300>) >= (ival<10, 20> || ival<250, 300>)));
}

TEST(safe_dsl_test, superset_v_subset_op) {
    EXPECT_TRUE(!(ival<0, 100> >= ival<0, 101>));
    EXPECT_TRUE(!(ival<0, 100> >= ival<200, 300>));
    EXPECT_TRUE(!(ival<0, 100> <= ival<200, 300>));
    EXPECT_TRUE(!(ival<0, 200> >= ival<100, 300>));
    EXPECT_TRUE(!(ival<0, 200> <= ival<100, 300>));
}

TEST(safe_dsl_test, complicated_expression_simplification) {
    EXPECT_EQ(
        ((ival<0, 10> || (ival<100, 200> + set<2, 7, 9, 42>) || ival<300, 600>) * (ival<20, 30> || ival<400, 800>)),
        (ival<0, 18000> || ival<40800, 480000>)
    );
}

TEST(safe_dsl_test, create_mask) {
    [[maybe_unused]] auto v = mask<0b1001>;
}

TEST(safe_dsl_test, mask_subset) {
    EXPECT_TRUE(mask<0b1111> >= mask<0b1010>);
    EXPECT_TRUE(mask<0b1111> >= mask<0b0000>);
    EXPECT_TRUE(mask<0b1111> >= mask<0b1111>);

    EXPECT_FALSE(mask<0b1001> >= mask<0b1010>);
    EXPECT_FALSE(mask<0b0001> >= mask<0b0010>);
    EXPECT_FALSE(mask<0b0001> >= mask<0b0011>);
}

TEST(safe_dsl_test, mask_superset) {
    EXPECT_TRUE(mask<0b1000> <= mask<0b1100>);
    EXPECT_TRUE(mask<0b0000> <= mask<0b0000>);
    EXPECT_TRUE(mask<0b0001> <= mask<0b1011>);

    EXPECT_FALSE(mask<0b1001> <= mask<0b1100>);
    EXPECT_FALSE(mask<0b0010> <= mask<0b0000>);
    EXPECT_FALSE(mask<0b0100> <= mask<0b1011>);
}

TEST(safe_dsl_test, mask_equal) {
    EXPECT_TRUE(mask<0b1000> == mask<0b1000>);
    EXPECT_TRUE(mask<0b0000> == mask<0b0000>);
    EXPECT_TRUE(mask<0b0001> == mask<0b0001>);

    EXPECT_FALSE(mask<0b1000> == mask<0b1001>);
    EXPECT_FALSE(mask<0b0011> == mask<0b0010>);
    EXPECT_FALSE(mask<0b0110> == mask<0b0100>);
}

TEST(safe_dsl_test, mask_not_equal) {
    EXPECT_FALSE(mask<0b1000> != mask<0b1000>);
    EXPECT_FALSE(mask<0b0000> != mask<0b0000>);
    EXPECT_FALSE(mask<0b0001> != mask<0b0001>);

    EXPECT_TRUE(mask<0b1000> != mask<0b1001>);
    EXPECT_TRUE(mask<0b0011> != mask<0b0010>);
    EXPECT_TRUE(mask<0b0110> != mask<0b0100>);
}

TEST(safe_dsl_test, mask_bitwise_or) {
    EXPECT_EQ(mask<0b0> | mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b1> | mask<0b0>, mask<0b1>);
    EXPECT_EQ(mask<0b0> | mask<0b1>, mask<0b1>);
    EXPECT_EQ(mask<0b1> | mask<0b1>, mask<0b1>);

    EXPECT_EQ(mask<0b01> | mask<0b10>, mask<0b11>);

    EXPECT_EQ(mask<0xba5e0000> | mask<0x0000ba11>, mask<0xba5eba11>);
}

TEST(safe_dsl_test, mask_bitwise_and) {
    EXPECT_EQ(mask<0b0> & mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b1> & mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b0> & mask<0b1>, mask<0b0>);
    EXPECT_EQ(mask<0b1> & mask<0b1>, mask<0b1>);

    EXPECT_EQ(mask<0b01> & mask<0b10>, mask<0b00>);
    EXPECT_EQ(mask<0b01> & mask<0b11>, mask<0b01>);
    EXPECT_EQ(mask<0b1110> & mask<0b0111>, mask<0b0110>);
}

TEST(safe_dsl_test, mask_bitwise_xor) {
    EXPECT_EQ(mask<0b0> ^ mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b1> ^ mask<0b0>, mask<0b1>);
    EXPECT_EQ(mask<0b0> ^ mask<0b1>, mask<0b1>);
    EXPECT_EQ(mask<0b1> ^ mask<0b1>, mask<0b1>);

    EXPECT_EQ(mask<0b01> ^ mask<0b10>, mask<0b11>);

    EXPECT_EQ(mask<0xba5e0000> ^ mask<0x0000ba11>, mask<0xba5eba11>);
}

TEST(safe_dsl_test, mask_bitwise_invert) {
    EXPECT_EQ(~mask<0>, (mask<0, ~0u>));
    EXPECT_EQ(~mask<0b1111>, (mask<0b1111, ~0u>));


    EXPECT_EQ(~mask<0> | mask<0>, (mask<0, ~0u>));


    EXPECT_NE(~mask<0>, mask<0>);
}


TEST(safe_dsl_test, mask_shift_left) {
    EXPECT_EQ((mask<0b0> << ival<0, 0>), mask<0b0>);
    EXPECT_EQ((mask<0b1> << ival<1, 1>), mask<0b10>);

    EXPECT_EQ((mask<0b1> << ival<0, 1>), mask<0b11>);
    EXPECT_EQ((mask<0b1> << ival<0, 4>), mask<0b11111>);
    EXPECT_EQ((mask<0b1> << ival<4, 8>), mask<0b111110000>);


    EXPECT_EQ((mask<0b1000001> << ival<0, 1>), mask<0b11000011>);
    EXPECT_EQ((mask<0b1000001> << ival<2, 4>), mask<0b11100011100>);
}


TEST(safe_dsl_test, mask_shift_right) {
    EXPECT_EQ((mask<0b1000> >> ival<0, 0>), mask<0b1000>);
    EXPECT_EQ((mask<0b1000> >> ival<1, 1>), mask<0b0100>);

    EXPECT_EQ((mask<0b1000> >> ival<0, 1>), mask<0b1100>);
    EXPECT_EQ((mask<0b10000000> >> ival<4, 6>), mask<0b1110>);
}


TEST(safe_dsl_test, detail_is_basic_mask) {
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

TEST(safe_dsl_test, mask_ival_eq) {
    EXPECT_TRUE((ival<0, 15> <= mask<0b1111>));
    EXPECT_TRUE((ival<0, 15> >= mask<0b1111>));

    EXPECT_TRUE((ival<0, 15> == mask<0b1111>));

    EXPECT_TRUE((ival<0, 14> >= mask<0b1110>));
    EXPECT_FALSE((ival<0, 14> <= mask<0b1110>));
    EXPECT_TRUE((ival<0, 14> != mask<0b1110>));
}

