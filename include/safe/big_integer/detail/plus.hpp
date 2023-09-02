#pragma once

#include <safe/big_integer/detail/algorithms.hpp>
#include <safe/big_integer/detail/bitwise.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <functional>

namespace safe::_big_integer::detail {
constexpr static auto plus =
    stateful_zip_transform(elem_t{}, [](elem_t &carry, double_elem_t const &lhs,
                                        double_elem_t const &rhs) {
        double_elem_t const result =
            lhs + rhs + static_cast<double_elem_t>(carry);

        carry = result >> 32u;
        return result & 0xffff'ffffu;
    });

constexpr static auto negate = [](auto &result, auto const &value) -> void {
    std::remove_cvref_t<decltype(result)> not_value{};
    bit_not(not_value, value);
    plus(result, not_value, make_storage(1));
};

constexpr static auto minus = [](auto &result, auto const &lhs,
                                 auto const &rhs) -> void {
    std::remove_cvref_t<decltype(result)> negative_rhs{};
    negate(negative_rhs, rhs);
    plus(result, lhs, negative_rhs);
};
} // namespace safe::_big_integer::detail
