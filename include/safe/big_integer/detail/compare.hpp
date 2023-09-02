#pragma once

#include <safe/big_integer/detail/plus.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <compare>
#include <cstdint>

namespace safe::_big_integer::detail {
template <std::size_t LhsNumBits, std::size_t RhsNumBits>
[[nodiscard]] constexpr auto unsigned_compare(storage<LhsNumBits> const &lhs,
                                              storage<RhsNumBits> const &rhs)
    -> std::strong_ordering {
    for (auto i = std::max(lhs.num_elems, rhs.num_elems); i > std::size_t{};) {
        --i;
        auto const l = lhs.get(i);
        auto const r = rhs.get(i);
        if (l < r) {
            return std::strong_ordering::less;
        }
        if (l > r) {
            return std::strong_ordering::greater;
        }
    }
    return std::strong_ordering::equal;
}

template <std::size_t LhsNumBits, std::size_t RhsNumBits>
[[nodiscard]] constexpr auto operator<=>(storage<LhsNumBits> const &lhs,
                                         storage<RhsNumBits> const &rhs)
    -> std::strong_ordering {
    if (lhs.negative()) {
        if (rhs.negative()) {
            return unsigned_compare(lhs, rhs);
        }
        return std::strong_ordering::less;
    }
    if (rhs.negative()) {
        return std::strong_ordering::greater;
    }
    return unsigned_compare(lhs, rhs);
}
} // namespace safe::_big_integer::detail
