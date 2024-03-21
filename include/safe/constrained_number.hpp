#pragma once

#include <safe/detail/assume.hpp>
#include <safe/detail/concepts.hpp>
#include <safe/detail/fwd.hpp>
#include <safe/detail/integral_type.hpp>
#include <safe/detail/pure.hpp>
#include <safe/detail/var_assign_static_assert.hpp>
#include <safe/dsl.hpp>
#include <safe/dsl/constraint_of.hpp>
#include <safe/dsl/eval.hpp>

#include <bit>
#include <compare>
#include <functional>
#include <limits>
#include <type_traits>

namespace safe {
template <auto Constraint, typename T> struct constrained_number {
  public:
    constexpr static auto constraint = Constraint;
    using value_type = T;

    constexpr constrained_number()
        requires(constraint >= constrain_interval<0, 0>)
        : _raw_value(0) {}

    template <typename U>
        requires(std::is_convertible_v<U, T>)
    // NOLINTNEXTLINE(google-explicit-constructor)
    SAFE_INLINE constexpr constrained_number(constraint_cast_ferry<U> ferry)
        : _raw_value{static_cast<T>(ferry.value())} {
        SAFE_ASSUME(constraint.check(_raw_value));
    }

    template <typename U>
        requires(std::integral<U> &&
                 (constraint >= detail::integral_type<U>::constraint))
    // NOLINTNEXTLINE(google-explicit-constructor)
    SAFE_INLINE constexpr constrained_number(U rhs) : _raw_value{rhs} {}

    template <typename U, U rhs>
        requires(constraint.check(rhs))
    // NOLINTNEXTLINE(google-explicit-constructor)
    SAFE_INLINE constexpr constrained_number(std::integral_constant<U, rhs>)
        : _raw_value(rhs) // intentionally allowing narrowing conversions
    {}

    // NOLINTNEXTLINE(google-explicit-constructor)
    SAFE_INLINE constexpr constrained_number(any_constrained auto const &rhs)
        : _raw_value(rhs.unsafe_value()) // intentionally allowing narrowing
                                            // conversions
    {
        static_assert_assign_requirements(*this, rhs);
    }

    SAFE_INLINE constexpr auto operator=(any_constrained auto const &rhs) -> constrained_number & {
        static_assert_assign_requirements(*this, rhs);
        _raw_value = rhs.unsafe_value();
        return *this;
    }

    SAFE_INLINE constexpr auto operator=(any_constrained auto &&rhs) -> constrained_number & {
        static_assert_assign_requirements(*this, rhs);
        _raw_value = rhs.unsafe_value();
        return *this;
    }

    // template<std::integral U>
    // requires (constraint <= dsl::constraint_of<U>)
    // [[nodiscard]] SAFE_INLINE constexpr operator U() {
    //     SAFE_ASSUME(constraint.check(_raw_value));
    //     return _raw_value;
    // }

    [[nodiscard]] SAFE_INLINE constexpr auto unsafe_value() const -> T {
        SAFE_ASSUME(constraint.check(_raw_value));
        return _raw_value;
    }

    using primitive_contract = detail::integral_type<T>;

    static_assert(rhs_must_be_subset_of_lhs<
                  lhs_req<decltype(primitive_contract::constraint)>,
                  rhs_req<decltype(Constraint)>>::value);

    /// @brief Do not use. Public to support use as non-type template parameter.
    T _raw_value;
};

template <typename... Ts> constexpr bool at_least_one_var = (... or any_constrained<Ts>);

[[nodiscard]] SAFE_INLINE constexpr auto to_var(any_constrained auto v) -> any_constrained auto {
    return v;
}

template <typename T, T v>
[[nodiscard]] SAFE_INLINE constexpr auto to_var(std::integral_constant<T, v>)
    -> any_constrained auto {
    return detail::make_constant<T, v>();
}

[[nodiscard]] SAFE_INLINE constexpr auto to_var(std::integral auto v) -> any_constrained
    auto {
    return value(v);
}

template <typename T>
concept var_admissable = requires(T t) {
    { to_var(t) } -> any_constrained;
};

namespace detail {
[[nodiscard]] SAFE_INLINE constexpr auto
bin_op(auto op, var_admissable auto raw_lhs, var_admissable auto raw_rhs) {
    any_constrained auto const lhs = to_var(raw_lhs);
    any_constrained auto const rhs = to_var(raw_rhs);

    auto result_req = dsl::detail::simp(op(lhs.constraint, rhs.constraint));

    // FIXME: replace with embiggening strategy; the result type is
    //        always large enough to contain its value

    auto result = op(lhs.unsafe_value(), rhs.unsafe_value());

    return constraint_cast<constrained_number<result_req, decltype(result)>>(result);
}
} // namespace detail

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator+(L lhs, R rhs) {
    return detail::bin_op(std::plus<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator-(L lhs, R rhs) {
    return detail::bin_op(std::minus<>(), lhs, rhs);
}

[[nodiscard]] SAFE_INLINE constexpr auto operator-(any_constrained auto v) {
    constexpr auto zero = constrained_number<constrain_interval<0, 0>, decltype(v.unsafe_value())>{};
    return zero - v;
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator*(L lhs, R rhs) {
    return detail::bin_op(std::multiplies<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator/(L lhs, R rhs) {
    return detail::bin_op(std::divides<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator%(L lhs, R rhs) {
    return detail::bin_op(std::modulus<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator<<(L lhs, R rhs) {
    return detail::bin_op([](auto a, auto b) { return a << b; }, lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator>>(L lhs, R rhs) {
    return detail::bin_op([](auto a, auto b) { return a >> b; }, lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator|(L lhs, R rhs) {
    return detail::bin_op(std::bit_or<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator&(L lhs, R rhs) {
    return detail::bin_op(std::bit_and<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator^(L lhs, R rhs) {
    return detail::bin_op(std::bit_xor<>(), lhs, rhs);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator<=>(L raw_lhs, R raw_rhs)
    -> std::compare_three_way_result_t<
        decltype(to_var(raw_lhs).unsafe_value()),
        decltype(to_var(raw_rhs).unsafe_value())> {
    return to_var(raw_lhs).unsafe_value() <=> to_var(raw_rhs).unsafe_value();
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator==(L raw_lhs, R raw_rhs)
    -> bool {
    return to_var(raw_lhs).unsafe_value() == to_var(raw_rhs).unsafe_value();
}

[[nodiscard]] SAFE_INLINE constexpr auto abs(any_constrained auto value) {
    using num_t = decltype(value.unsafe_value());
    auto result = static_cast<num_t>(std::abs(value.unsafe_value()));
    auto result_req = dsl::detail::simp(safe::dsl::abs(value.constraint));
    return constraint_cast<constrained_number<result_req, num_t>>(result);
}

[[nodiscard]] SAFE_INLINE constexpr auto bit_width(any_constrained auto value) {
    using num_t = decltype(value.unsafe_value());
    auto result = static_cast<num_t>(std::bit_width(value.unsafe_value()));
    auto result_req =
        dsl::detail::simp(safe::dsl::bit_width(value.constraint));
    return constraint_cast<constrained_number<result_req, num_t>>(result);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto min(L raw_lhs, R raw_rhs) -> any_constrained auto {
    any_constrained auto const lhs = to_var(raw_lhs);
    any_constrained auto const rhs = to_var(raw_rhs);

    using common_type = std::common_type_t<decltype(lhs.unsafe_value()),
                                           decltype(rhs.unsafe_value())>;
    auto result = std::min<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
    using result_t = decltype(result);
    auto result_req =
        dsl::detail::simp(dsl::min(lhs.constraint, rhs.constraint));
    return constraint_cast<constrained_number<result_req, result_t>>(result);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto max(L raw_lhs, R raw_rhs) -> any_constrained auto {
    any_constrained auto const lhs = to_var(raw_lhs);
    any_constrained auto const rhs = to_var(raw_rhs);

    using common_type = std::common_type_t<decltype(lhs.unsafe_value()),
                                           decltype(rhs.unsafe_value())>;
    auto result = std::max<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
    using result_t = decltype(result);
    auto result_req =
        dsl::detail::simp(dsl::max(lhs.constraint, rhs.constraint));
    return constraint_cast<constrained_number<result_req, result_t>>(result);
}

template <var_admissable T, var_admissable MinT, var_admissable MaxT>
    requires(at_least_one_var<T, MinT, MaxT>)
[[nodiscard]] SAFE_INLINE constexpr auto clamp(T value, MinT min_val,
                                               MaxT max_val) -> any_constrained auto {
    return min(max(to_var(value), to_var(min_val)), to_var(max_val));
}
} // namespace safe
