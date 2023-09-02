#pragma once

#include <safe/big_integer/detail/algorithms.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <functional>

namespace safe::_big_integer::detail {
constexpr static auto bit_and = zip_transform(std::bit_and{});
constexpr static auto bit_or = zip_transform(std::bit_or{});
constexpr static auto bit_xor = zip_transform(std::bit_xor{});
constexpr static auto bit_not = transform(std::bit_not{});
} // namespace safe::_big_integer::detail