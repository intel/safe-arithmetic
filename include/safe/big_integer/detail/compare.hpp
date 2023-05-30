#pragma once


#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/plus.hpp>

#include <compare>
#include <cstdint>


namespace safe::_big_integer::detail {
    template<
        std::size_t LhsNumBits,
        std::size_t RhsNumBits>
    [[nodiscard]] constexpr auto unsigned_compare(
        storage<LhsNumBits> const & lhs,
        storage<RhsNumBits> const & rhs
    ) -> std::strong_ordering {
        auto i = std::max(lhs.num_elems, rhs.num_elems);
        do {
            i--;

            if (lhs.get(i) < rhs.get(i)) {
                return std::strong_ordering::less;

            } else if (lhs.get(i) > rhs.get(i)) {
                return std::strong_ordering::greater;
            }
        } while (i > 0);

        return std::strong_ordering::equal;
    }

    template<
        std::size_t LhsNumBits,
        std::size_t RhsNumBits>
    [[nodiscard]] constexpr auto operator<=>(
        storage<LhsNumBits> const & lhs,
        storage<RhsNumBits> const & rhs
    ) -> std::strong_ordering {
        if (lhs.negative()) {
            if (rhs.negative()) {
                return unsigned_compare(lhs, rhs);
            } else {
                return std::strong_ordering::less;
            }
        } else {
            if (rhs.negative()) {
                return std::strong_ordering::greater;
            } else {
                return unsigned_compare(lhs, rhs);
            }
        }
    }
}