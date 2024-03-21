#pragma once

#include <safe/dsl.hpp>
#include <safe/constrained_number.hpp>

namespace safe::detail {
template <typename U, U value>
[[nodiscard]] constexpr inline auto make_constant() {
    return constraint_cast<constrained_number<constrain_interval<value, value>, U>>(value);
}
} // namespace safe::detail