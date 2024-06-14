#pragma once

#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/is_subset.hpp>
#include <safe/dsl/constrain_interval.hpp>

namespace safe::dsl {
template <typename T, typename U> using is_superset = is_subset<U, T>;

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator>=(LhsT, RhsT) -> bool {
    return static_cast<bool>(detail::eval_v<is_superset<LhsT, RhsT>>);
}
} // namespace safe::dsl
