#pragma once

#include <safe/big_integer/detail/algorithms.hpp>
#include <safe/big_integer/detail/plus.hpp>
#include <safe/big_integer/detail/shift.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <concepts>
#include <functional>

namespace safe::_big_integer::detail {
constexpr static auto unsigned_multiplies = [](auto &result, auto const &lhs,
                                               auto const &rhs) -> void {
    using result_t = std::remove_cvref_t<decltype(result)>;

    for (int i = 0; i < lhs.num_elems; i++) {
        for (int j = 0; j < rhs.num_elems; j++) {
            auto const raw_partial_product =
                static_cast<double_elem_t>(lhs.get(i)) *
                static_cast<double_elem_t>(rhs.get(j));

            result_t partial_product{
                {static_cast<elem_t>(raw_partial_product & 0xffff'ffffu),
                 static_cast<elem_t>(raw_partial_product >> 32u)}};

            bit_shift_left(partial_product, partial_product, (i + j) * 32);

            plus(result, result, partial_product);
        }
    }
};

constexpr static auto multiplies = [](auto &result, auto const &lhs,
                                      auto const &rhs) -> void {
    using lhs_t = std::remove_cvref_t<decltype(result)>;
    using rhs_t = std::remove_cvref_t<decltype(result)>;

    if (lhs.negative()) {
        lhs_t pos_lhs{};
        negate(pos_lhs, lhs);

        if (rhs.negative()) {
            rhs_t pos_rhs{};
            negate(pos_rhs, rhs);
            unsigned_multiplies(result, pos_lhs, pos_rhs);
        } else {
            unsigned_multiplies(result, pos_lhs, rhs);
            negate(result, result);
        }
    } else {
        if (rhs.negative()) {
            rhs_t pos_rhs{};
            negate(pos_rhs, rhs);
            unsigned_multiplies(result, lhs, pos_rhs);
            negate(result, result);

        } else {
            unsigned_multiplies(result, lhs, rhs);
        }
    }
};
} // namespace safe::_big_integer::detail
