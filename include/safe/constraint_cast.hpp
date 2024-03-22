#pragma once

#include <safe/detail/fwd.hpp>

#include <concepts>

namespace safe {
namespace detail {
template <typename T> struct constraint_cast_ferry {
  private:
    T v;

  public:
    SAFE_INLINE constexpr explicit(true) constraint_cast_ferry(T new_value)
        : v{new_value} {}

    [[nodiscard]] SAFE_INLINE constexpr auto value() const -> T { return v; }
};    
} // namespace detail

template <any_constraint auto C, std::integral T>
[[nodiscard]] constexpr auto constraint_cast(T const & src) -> constrained_number<C, T> {
    return {detail::constraint_cast_ferry{src}};
}

} // namespace safe