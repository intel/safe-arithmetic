#pragma once

#include <safe/big_integer/detail/storage.hpp>

#include <algorithm>
#include <cstdint>

namespace safe::_big_integer::detail {
[[nodiscard]] constexpr auto reverse_zip_transform(auto op) {
    return [=](auto &result, auto const &lhs, auto const &rhs) -> void {
        auto i = result.num_elems;
        do {
            i--;
            result.set(i, op(lhs.get(i), rhs.get(i)));
        } while (i > std::size_t{});
    };
}

[[nodiscard]] constexpr auto zip_transform(auto op) {
    return [=](auto &result, auto const &lhs, auto const &rhs) -> void {
        for (auto i = std::size_t{}; i < result.num_elems; i++) {
            result.set(i, op(lhs.get(i), rhs.get(i)));
        }
    };
}

[[nodiscard]] constexpr auto stateful_zip_transform(auto initial_value,
                                                    auto op) {
    return [=](auto &result, auto const &lhs, auto const &rhs) -> void {
        auto state = initial_value;
        for (auto i = std::size_t{}; i < result.num_elems; i++) {
            result.set(i, op(state, lhs.get(i), rhs.get(i)));
        }
    };
}

[[nodiscard]] constexpr auto transform(auto op) {
    return [=](auto &result, auto const &value) -> void {
        for (auto i = std::size_t{}; i < result.num_elems; i++) {
            result.set(i, op(value.get(i)));
        }
    };
}
} // namespace safe::_big_integer::detail