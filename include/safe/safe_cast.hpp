#pragma once

#include <safe/detail/concepts.hpp>
#include <safe/detail/fwd.hpp>
#include <safe/var.hpp>

#include <concepts>

template <typename To, safe::Var From>
    requires(std::is_convertible_v<typename From::value_type, To>)
[[nodiscard]] SAFE_INLINE constexpr To safe_cast(From const &src) {
    static_assert(safe::detail::integral_type<To>::requirement >=
                      From::requirement,
                  "The safe value must fit within the target value type.");

    return static_cast<To>(src.unsafe_value_);
}
