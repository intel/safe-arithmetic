#include <safe.hpp>

#include <type_traits>

template<typename T>
struct show_type : public std::integral_constant<bool, false> {};

using safe::int32_;

void test_clamped_value() {
    static_assert(safe::clamp<int, 0, 10>(11).unsafe_value() == 10);
    static_assert(safe::clamp<int, 0, 10>(-1).unsafe_value() == 0);

    constexpr auto five = safe::clamp<int, 0, 10>(5);
    static_assert(five.unsafe_value() == 5);

    constexpr safe::contract<int, safe::interval<0, 10>> explicit_five = five;
    static_assert(explicit_five.unsafe_value() == 5);

    static_assert(std::is_same_v<decltype(five), const safe::contract<int, safe::interval<0, 10>>>);

    safe::int32<0, 1000> counter = safe::constant<int32_t, 0>;

    counter = safe::check_and_clamp(counter + int32_<1>);

    counter = (counter + int32_<1>) % int32_<512>;
}

void test_plus() {
    constexpr auto sum = safe::clamp<int, 0, 10>(8) + safe::clamp<int, 0, 10>(2);
    static_assert(sum.unsafe_value() == 10);

    static_assert(std::is_same_v<decltype(sum), const safe::contract<int, safe::interval<0, 20>>>);
}

void test_minus() {
    constexpr auto difference = safe::clamp<int, 0, 10>(8) - safe::clamp<int, 0, 10>(2);
    static_assert(difference.unsafe_value() == 6);

    static_assert(std::is_same_v<decltype(difference), const safe::contract<int, safe::interval<-10, 10>>>);
}

void test_negate() {
    constexpr auto negate = -safe::clamp<int, 0, 10>(2);
    static_assert(negate.unsafe_value() == -2);

    static_assert(std::is_same_v<decltype(negate), const safe::contract<int, safe::interval<-10, 0>>>);
}

template<int lhs_min, int lhs_max, int rhs_min, int rhs_max, int result_min, int result_max>
void assert_mult() {
    static_assert(std::is_same_v<
        decltype(safe::clamp<int, lhs_min, lhs_max>(0) * safe::clamp<int, rhs_min, rhs_max>(0)),
        safe::contract<int, safe::interval<result_min, result_max>>>);
}

void test_multiplication() {
    assert_mult<  0,   0,   -10, -10,       0,    0>();
    assert_mult<  0,   0,   -10,   0,       0,    0>();
    assert_mult<  0,   0,   -10,  10,       0,    0>();
    assert_mult<  0,   0,     0,   0,       0,    0>();
    assert_mult<  0,   0,     0,  10,       0,    0>();
    assert_mult<  0,   0,    10,  10,       0,    0>();

    assert_mult<-10, -10,   -10, -10,     100,  100>();
    assert_mult<-10, -10,   -10,   0,       0,  100>();
    assert_mult<-10, -10,   -10,  10,    -100,  100>();
    assert_mult<-10, -10,     0,   0,       0,    0>();
    assert_mult<-10, -10,     0,  10,    -100,    0>();
    assert_mult<-10, -10,    10,  10,    -100, -100>();

    assert_mult<-10,   0,   -10, -10,       0,  100>();
    assert_mult<-10,   0,   -10,   0,       0,  100>();
    assert_mult<-10,   0,   -10,  10,    -100,  100>();
    assert_mult<-10,   0,     0,   0,       0,    0>();
    assert_mult<-10,   0,     0,  10,    -100,    0>();
    assert_mult<-10,   0,    10,  10,    -100,    0>();

    assert_mult<-10,  10,   -10, -10,    -100,  100>();
    assert_mult<-10,  10,   -10,   0,    -100,  100>();
    assert_mult<-10,  10,   -10,  10,    -100,  100>();
    assert_mult<-10,  10,     0,   0,       0,    0>();
    assert_mult<-10,  10,     0,  10,    -100,  100>();
    assert_mult<-10,  10,    10,  10,    -100,  100>();

    assert_mult<  0,   0,   -10, -10,       0,    0>();
    assert_mult<  0,   0,   -10,   0,       0,    0>();
    assert_mult<  0,   0,   -10,  10,       0,    0>();
    assert_mult<  0,   0,     0,   0,       0,    0>();
    assert_mult<  0,   0,     0,  10,       0,    0>();
    assert_mult<  0,   0,    10,  10,       0,    0>();

    assert_mult<  0,  10,   -10, -10,    -100,    0>();
    assert_mult<  0,  10,   -10,   0,    -100,    0>();
    assert_mult<  0,  10,   -10,  10,    -100,  100>();
    assert_mult<  0,  10,     0,   0,       0,    0>();
    assert_mult<  0,  10,     0,  10,       0,  100>();
    assert_mult<  0,  10,    10,  10,       0,  100>();

    assert_mult< 10,  10,   -10, -10,    -100, -100>();
    assert_mult< 10,  10,   -10,   0,    -100,    0>();
    assert_mult< 10,  10,   -10,  10,    -100,  100>();
    assert_mult< 10,  10,     0,   0,       0,    0>();
    assert_mult< 10,  10,     0,  10,       0,  100>();
    assert_mult< 10,  10,    10,  10,     100,  100>();
}

constexpr void add_two_numbers(
    safe::int32<0, 10> a,
    safe::int32<0, 10> b
) {
    a + b;
}

void check_test() {
    constexpr bool pass = safe::invoke(add_two_numbers, 2, 5);
    static_assert(pass);

    constexpr bool fail = safe::invoke(add_two_numbers, 12, 5);
    static_assert(!fail);
}

int main() {
    return 0;
}