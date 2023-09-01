#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(storage, assigned_value_equals_original, (storage<128> a)) {
    assigned_value_equals_original<128>(a);
    assigned_value_equals_original<256>(a);
}

RC_GTEST_PROP(storage, copied_value_equals_original, (storage<128> a)) {
    copied_value_equals_original<128>(a);
    copied_value_equals_original<256>(a);
}

RC_GTEST_PROP(storage, get_set_is_consistent,
              (uint32_t value, storage<512> a)) {
    auto const i = *rc::gen::inRange<std::size_t>(0, a.num_elems);
    a.set(i, value);
    RC_ASSERT(a.get(i) == value);
}

TEST(storage, default_construction) {
    storage<32> s{};
    ASSERT_EQ(s.get(0), 0u);
}

TEST(storage, num_elems) {
    ASSERT_EQ(storage<129>::num_elems, 5);
    ASSERT_EQ(storage<128>::num_elems, 4);
    ASSERT_EQ(storage<127>::num_elems, 4);

    ASSERT_EQ(storage<97>::num_elems, 4);
    ASSERT_EQ(storage<96>::num_elems, 3);
    ASSERT_EQ(storage<95>::num_elems, 3);

    ASSERT_EQ(storage<33>::num_elems, 2);
    ASSERT_EQ(storage<32>::num_elems, 1);
    ASSERT_EQ(storage<1>::num_elems, 1);
}

TEST(storage, equality) {
    ASSERT_EQ(storage<32>{{42u}}, storage<32>{{42u}});
    ASSERT_NE(storage<32>{{42u}}, storage<32>{{13u}});

    ASSERT_EQ(storage<32>{{0x8000'0000u}},
              (storage<64>{{0x8000'0000, 0xffff'ffffu}}));
    ASSERT_NE(storage<32>{{0x8000'0000u}},
              (storage<64>{{0x8000'0000, 0x0000'0000u}}));
    ASSERT_EQ(storage<32>{{0x7000'0000u}},
              (storage<64>{{0x7000'0000, 0x0000'0000u}}));
    ASSERT_NE(storage<32>{{0x7000'0000u}},
              (storage<64>{{0x7000'0000, 0xffff'ffffu}}));
}

TEST(storage, in_bound_get_set) {
    storage<128> v{};

    v.set(0, 0xba5e'ba11u);
    v.set(1, 0x57ac'ca70u);
    v.set(2, 0x1111'1111u);
    v.set(3, 0x7777'7777u);

    ASSERT_EQ(v.get(0), 0xba5e'ba11u);
    ASSERT_EQ(v.get(1), 0x57ac'ca70u);
    ASSERT_EQ(v.get(2), 0x1111'1111u);
    ASSERT_EQ(v.get(3), 0x7777'7777u);
}

TEST(storage, out_bound_positive_get_set) {
    storage<128> v{};

    v.set(4, 0xba5e'ba11u);
    v.set(5, 0x57ac'ca70u);
    v.set(6, 0x1111'1111u);
    v.set(7, 0x7777'7777u);

    ASSERT_EQ(v.get(4), 0u);
    ASSERT_EQ(v.get(5), 0u);
    ASSERT_EQ(v.get(6), 0u);
    ASSERT_EQ(v.get(7), 0u);
}

TEST(storage, negative) {
    storage<128> v{};

    v.set(3, 0x8000'0000u);
    ASSERT_EQ(v.negative(), true);

    v.set(3, 0x7fff'ffffu);
    ASSERT_EQ(v.negative(), false);
}

TEST(storage, out_bound_negative_get_set) {
    storage<128> v{};

    v.set(3, 0x8000'0000u);
    v.set(4, 0xba5e'ba11u);
    v.set(5, 0x57ac'ca70u);
    v.set(6, 0x1111'1111u);

    ASSERT_EQ(v.get(3), 0x8000'0000u);
    ASSERT_EQ(v.get(4), 0xffff'ffffu);
    ASSERT_EQ(v.get(5), 0xffff'ffffu);
    ASSERT_EQ(v.get(6), 0xffff'ffffu);
    ASSERT_EQ(v.get(7), 0xffff'ffffu);
}

TEST(storage, array_constructor) {
    storage<128> v{{42u, 43u, 44u, 45u}};

    ASSERT_EQ(v.get(0), 42u);
    ASSERT_EQ(v.get(1), 43u);
    ASSERT_EQ(v.get(2), 44u);
    ASSERT_EQ(v.get(3), 45u);
}

TEST(storage, copy_constructor) {
    storage<128> v1{{42u, 43u, 44u, 45u}};
    storage<128> v2{v1};

    ASSERT_EQ(v2.get(0), 42u);
    ASSERT_EQ(v2.get(1), 43u);
    ASSERT_EQ(v2.get(2), 44u);
    ASSERT_EQ(v2.get(3), 45u);
}

TEST(storage, copy_assignment) {
    storage<128> v1{{42u, 43u, 44u, 45u}};
    storage<128> v2{};
    v2 = v1;

    ASSERT_EQ(v2.get(0), 42u);
    ASSERT_EQ(v2.get(1), 43u);
    ASSERT_EQ(v2.get(2), 44u);
    ASSERT_EQ(v2.get(3), 45u);
}

TEST(storage, assign_to_larger) {
    storage<128> v1{{42u, 43u, 44u, 45u}};
    storage<256> v2{};
    v2 = v1;

    ASSERT_EQ(v2.get(0), 42u);
    ASSERT_EQ(v2.get(1), 43u);
    ASSERT_EQ(v2.get(2), 44u);
    ASSERT_EQ(v2.get(3), 45u);
    ASSERT_EQ(v2.get(4), 0u);
    ASSERT_EQ(v2.get(5), 0u);
    ASSERT_EQ(v2.get(6), 0u);
    ASSERT_EQ(v2.get(7), 0u);
}
} // namespace safe::_big_integer::detail