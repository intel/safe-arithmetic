#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/compare.hpp>
#include <safe/big_integer/detail/plus.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(compare, equality_256, (storage<256> a)) {
    RC_ASSERT((a <=> a) == std::strong_ordering::equal);
}

RC_GTEST_PROP(compare, equality_32, (storage<32> a)) {
    RC_ASSERT((a <=> a) == std::strong_ordering::equal);
}

RC_GTEST_PROP(compare, asymmetric_equality_256, (storage<256> a)) {
    storage<512> b{a};
    RC_ASSERT((a <=> b) == std::strong_ordering::equal);
}

RC_GTEST_PROP(compare, inequality_by_plus, (storage<256> a)) {
    storage<257> b{};
    plus(b, a, make_storage(1));
    RC_ASSERT((a <=> b) == std::strong_ordering::less);
    RC_ASSERT((b <=> a) == std::strong_ordering::greater);
}

RC_GTEST_PROP(compare, is_compare_64, (int64_t a, int64_t b)) {
    std::strong_ordering const actual = make_storage(a) <=> make_storage(b);
    std::strong_ordering const expected = (a <=> b);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(compare, is_compare_32, (int32_t a, int32_t b)) {
    std::strong_ordering const actual = make_storage(a) <=> make_storage(b);
    std::strong_ordering const expected = (a <=> b);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(compare, is_compare_32_64, (int32_t a, int64_t b)) {
    std::strong_ordering const actual = make_storage(a) <=> make_storage(b);
    std::strong_ordering const expected = (a <=> b);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(compare, is_compare_64_32, (int64_t a, int32_t b)) {
    std::strong_ordering const actual = make_storage(a) <=> make_storage(b);
    std::strong_ordering const expected = (a <=> b);
    RC_ASSERT(actual == expected);
}
} // namespace safe::_big_integer::detail