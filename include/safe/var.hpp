#pragma once

#include <safe/detail/assume.hpp>
#include <safe/detail/concepts.hpp>
#include <safe/detail/fwd.hpp>
#include <safe/detail/integral_type.hpp>
#include <safe/detail/pure.hpp>
#include <safe/detail/var_assign_static_assert.hpp>
#include <safe/dsl.hpp>
#include <safe/dsl/eval.hpp>

#include <bit>
#include <compare>
#include <functional>
#include <limits>
#include <type_traits>

namespace safe {
template <typename T, auto Requirement> struct var {
  public:
    constexpr static auto requirement = Requirement;
    using value_type = T;

    constexpr var()
        requires(requirement >= ival<0, 0>)
        : unsafe_value_(0) {}

    template <typename U>
        requires(std::is_convertible_v<U, T>)
    SAFE_INLINE constexpr var(unsafe_cast_ferry<U> ferry)
        : unsafe_value_{static_cast<T>(ferry.value())} {
        SAFE_ASSUME(requirement.check(unsafe_value_));
    }

    template <typename U>
        requires(std::integral<U> &&
                 (requirement >= detail::integral_type<U>::requirement))
    SAFE_INLINE constexpr var(U rhs) : unsafe_value_{rhs} {}

    template <typename U, U rhs>
        requires(requirement.check(rhs))
    SAFE_INLINE constexpr var(std::integral_constant<U, rhs>)
        : unsafe_value_(rhs) // intentionally allowing narrowing conversions
    {}

    SAFE_INLINE constexpr var(Var auto const &rhs)
        : unsafe_value_(rhs.unsafe_value()) // intentionally allowing narrowing
                                            // conversions
    {
        static_assert_assign_requirements(*this, rhs);
    }

    SAFE_INLINE constexpr var(Var auto &&rhs)
        : unsafe_value_(rhs.unsafe_value()) // intentionally allowing narrowing
                                            // conversions
    {
        static_assert_assign_requirements(*this, rhs);
    }

    SAFE_INLINE constexpr auto operator=(Var auto const &rhs) -> var & {
        static_assert_assign_requirements(*this, rhs);
        unsafe_value_ = rhs.unsafe_value();
        return *this;
    }

    SAFE_INLINE constexpr auto operator=(Var auto &&rhs) -> var & {
        static_assert_assign_requirements(*this, rhs);
        unsafe_value_ = rhs.unsafe_value();
        return *this;
    }

    [[nodiscard]] SAFE_INLINE constexpr T unsafe_value() const {
        SAFE_ASSUME(requirement.check(unsafe_value_));
        return unsafe_value_;
    }

    using primitive_contract = detail::integral_type<T>;

    static_assert(rhs_must_be_subset_of_lhs<
                  lhs_req<decltype(primitive_contract::requirement)>,
                  rhs_req<decltype(Requirement)>>::value);

    /// @brief Do not use. Public to support use as non-type template parameter.
    T unsafe_value_;
};

template <typename... Ts> constexpr bool at_least_one_var = (... or Var<Ts>);

[[nodiscard]] SAFE_INLINE constexpr auto to_var(Var auto v) -> Var auto {
    return v;
}

template <typename T, T v>
[[nodiscard]] SAFE_INLINE constexpr auto to_var(std::integral_constant<T, v>)
    -> Var auto {
    return detail::make_constant<T, v>();
}

[[nodiscard]] SAFE_INLINE constexpr auto to_var(std::integral auto v) -> Var
    auto {
    return value(v);
}

template <typename T>
concept var_admissable = requires(T t) {
    { to_var(t) } -> Var;
};

namespace detail {
[[nodiscard]] SAFE_INLINE constexpr auto
bin_op(auto op, var_admissable auto raw_lhs, var_admissable auto raw_rhs) {
    Var auto const lhs = to_var(raw_lhs);
    Var auto const rhs = to_var(raw_rhs);

    auto result_req = dsl::detail::simp(op(lhs.requirement, rhs.requirement));

    // FIXME: replace with embiggening strategy; the result type is
    //        always large enough to contain its value

    auto result = op(lhs.unsafe_value(), rhs.unsafe_value());

    return unsafe_cast<var<decltype(result), result_req>>(result);
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

[[nodiscard]] SAFE_INLINE constexpr auto operator-(Var auto v) {
    constexpr auto zero = var<decltype(v.unsafe_value()), ival<0, 0>>{};
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

[[nodiscard]] SAFE_INLINE constexpr auto abs(Var auto value) {
    using num_t = decltype(value.unsafe_value());
    auto result = static_cast<num_t>(std::abs(value.unsafe_value()));
    auto result_req = dsl::detail::simp(safe::dsl::abs(value.requirement));
    return unsafe_cast<var<num_t, result_req>>(result);
}

[[nodiscard]] SAFE_INLINE constexpr auto bit_width(Var auto value) {
    using num_t = decltype(value.unsafe_value());
    auto result = static_cast<num_t>(std::bit_width(value.unsafe_value()));
    auto result_req =
        dsl::detail::simp(safe::dsl::bit_width(value.requirement));
    return unsafe_cast<var<num_t, result_req>>(result);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto min(L raw_lhs, R raw_rhs) -> Var auto {
    Var auto const lhs = to_var(raw_lhs);
    Var auto const rhs = to_var(raw_rhs);

    using common_type = std::common_type_t<decltype(lhs.unsafe_value()),
                                           decltype(rhs.unsafe_value())>;
    auto result = std::min<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
    using result_t = decltype(result);
    auto result_req =
        dsl::detail::simp(dsl::min(lhs.requirement, rhs.requirement));
    return unsafe_cast<var<result_t, result_req>>(result);
}

template <var_admissable L, var_admissable R>
    requires(at_least_one_var<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto max(L raw_lhs, R raw_rhs) -> Var auto {
    Var auto const lhs = to_var(raw_lhs);
    Var auto const rhs = to_var(raw_rhs);

    using common_type = std::common_type_t<decltype(lhs.unsafe_value()),
                                           decltype(rhs.unsafe_value())>;
    auto result = std::max<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
    using result_t = decltype(result);
    auto result_req =
        dsl::detail::simp(dsl::max(lhs.requirement, rhs.requirement));
    return unsafe_cast<var<result_t, result_req>>(result);
}

template <var_admissable T, var_admissable MinT, var_admissable MaxT>
    requires(at_least_one_var<T, MinT, MaxT>)
[[nodiscard]] SAFE_INLINE constexpr auto clamp(T value, MinT min_val,
                                               MaxT max_val) -> Var auto {
    return min(max(to_var(value), to_var(min_val)), to_var(max_val));
}
} // namespace safe
