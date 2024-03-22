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
#include <safe/constraint_cast.hpp>

#include <bit>
#include <compare>
#include <functional>
#include <limits>
#include <type_traits>

namespace safe {
template <any_constraint auto C, typename T> struct constrained_number {
  public:
    constexpr static auto constraint = C;
    using value_type = T;

    constexpr constrained_number()
        requires(constraint >= constrain_set<0>)
        : _raw_value(0) {}

    template <typename U>
        requires(std::is_convertible_v<U, T>)
    // NOLINTNEXTLINE(google-explicit-constructor)
    SAFE_INLINE constexpr constrained_number(detail::constraint_cast_ferry<U> ferry)
        : _raw_value{static_cast<T>(ferry.value())} {
        SAFE_ASSUME(constraint.check(_raw_value));
    }

    template <typename U>
        requires(std::integral<U> &&
                 (constraint >= constraint_of<U>))
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
        : _raw_value(rhs.raw_value()) // intentionally allowing narrowing
                                            // conversions
    {
        static_assert_assign_requirements(*this, rhs);
    }

    SAFE_INLINE constexpr auto operator=(any_constrained auto const &rhs) -> constrained_number & {
        static_assert_assign_requirements(*this, rhs);
        _raw_value = rhs.raw_value();
        return *this;
    }

    SAFE_INLINE constexpr auto operator=(any_constrained auto &&rhs) -> constrained_number & {
        static_assert_assign_requirements(*this, rhs);
        _raw_value = rhs.raw_value();
        return *this;
    }

    template<std::integral U>
    requires (constraint <= constraint_of<U>)
    [[nodiscard]] SAFE_INLINE constexpr operator U() {
        SAFE_ASSUME(constraint.check(_raw_value));
        return _raw_value;
    }

    [[nodiscard]] SAFE_INLINE constexpr auto raw_value() const -> T {
        SAFE_ASSUME(constraint.check(_raw_value));
        return _raw_value;
    }

    static_assert(constraint <= constraint_of<T>);

    /// @brief Do not use. Public to support use as non-type template parameter.
    T _raw_value;
};

template<std::integral T>
constrained_number(T) -> constrained_number<constraint_of<T>, T>;

template<std::integral T, T v>
constrained_number(std::integral_constant<T, v>) -> constrained_number<constrain_interval<v, v>, T>;


template <typename... Ts> constexpr bool at_least_one_cnum = (... or any_constrained<Ts>);



template <typename T>
concept cnum_admissable = requires(T t) {
    { constrained_number(t) } -> any_constrained;
};

namespace detail {
[[nodiscard]] SAFE_INLINE constexpr auto
bin_op(auto op, cnum_admissable auto raw_lhs, cnum_admissable auto raw_rhs) {
    auto const lhs = constrained_number(raw_lhs);
    auto const rhs = constrained_number(raw_rhs);

    auto result_req = dsl::detail::simp(op(lhs.constraint, rhs.constraint));

    // FIXME: replace with embiggening strategy; the result type is
    //        always large enough to contain its value

    auto result = op(lhs.raw_value(), rhs.raw_value());

    return constraint_cast<result_req, decltype(result)>(result);
}
} // namespace detail

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator+(L lhs, R rhs) {
    return detail::bin_op(std::plus<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator-(L lhs, R rhs) {
    return detail::bin_op(std::minus<>(), lhs, rhs);
}

[[nodiscard]] SAFE_INLINE constexpr auto operator-(any_constrained auto v) {
    constexpr auto zero = constrained_number<constrain_interval<0, 0>, decltype(v.raw_value())>{};
    return zero - v;
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator*(L lhs, R rhs) {
    return detail::bin_op(std::multiplies<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator/(L lhs, R rhs) {
    return detail::bin_op(std::divides<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator%(L lhs, R rhs) {
    return detail::bin_op(std::modulus<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator<<(L lhs, R rhs) {
    return detail::bin_op([](auto a, auto b) { return a << b; }, lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator>>(L lhs, R rhs) {
    return detail::bin_op([](auto a, auto b) { return a >> b; }, lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator|(L lhs, R rhs) {
    return detail::bin_op(std::bit_or<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator&(L lhs, R rhs) {
    return detail::bin_op(std::bit_and<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator^(L lhs, R rhs) {
    return detail::bin_op(std::bit_xor<>(), lhs, rhs);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator<=>(L raw_lhs, R raw_rhs)
    -> std::compare_three_way_result_t<
        decltype(constrained_number(raw_lhs).raw_value()),
        decltype(constrained_number(raw_rhs).raw_value())> {
    return constrained_number(raw_lhs).raw_value() <=> constrained_number(raw_rhs).raw_value();
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto operator==(L raw_lhs, R raw_rhs)
    -> bool {
    return constrained_number(raw_lhs).raw_value() == constrained_number(raw_rhs).raw_value();
}

[[nodiscard]] SAFE_INLINE constexpr auto abs(any_constrained auto value) {
    using num_t = decltype(value.raw_value());
    auto result = static_cast<num_t>(std::abs(value.raw_value()));
    auto result_req = dsl::detail::simp(safe::dsl::abs(value.constraint));
    return constraint_cast<result_req, num_t>(result);
}

[[nodiscard]] SAFE_INLINE constexpr auto bit_width(any_constrained auto value) {
    using num_t = decltype(value.raw_value());
    auto result = static_cast<num_t>(std::bit_width(value.raw_value()));
    auto result_req =
        dsl::detail::simp(safe::dsl::bit_width(value.constraint));
    return constraint_cast<result_req, num_t>(result);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto min(L raw_lhs, R raw_rhs) -> any_constrained auto {
    auto const lhs = constrained_number(raw_lhs);
    auto const rhs = constrained_number(raw_rhs);

    using common_type = std::common_type_t<decltype(lhs.raw_value()),
                                           decltype(rhs.raw_value())>;
    auto result = std::min<common_type>(lhs.raw_value(), rhs.raw_value());
    using result_t = decltype(result);
    auto result_req =
        dsl::detail::simp(dsl::min(lhs.constraint, rhs.constraint));
    return constraint_cast<result_req, result_t>(result);
}

template <cnum_admissable L, cnum_admissable R>
    requires(at_least_one_cnum<L, R>)
[[nodiscard]] SAFE_INLINE constexpr auto max(L raw_lhs, R raw_rhs) -> any_constrained auto {
    auto const lhs = constrained_number(raw_lhs);
    auto const rhs = constrained_number(raw_rhs);

    using common_type = std::common_type_t<decltype(lhs.raw_value()),
                                           decltype(rhs.raw_value())>;
    auto result = std::max<common_type>(lhs.raw_value(), rhs.raw_value());
    using result_t = decltype(result);
    auto result_req =
        dsl::detail::simp(dsl::max(lhs.constraint, rhs.constraint));
    return constraint_cast<result_req, result_t>(result);
}

template <cnum_admissable T, cnum_admissable MinT, cnum_admissable MaxT>
    requires(at_least_one_cnum<T, MinT, MaxT>)
[[nodiscard]] SAFE_INLINE constexpr auto clamp(T value, MinT min_val,
                                               MaxT max_val) -> any_constrained auto {
    return min(max(constrained_number(value), constrained_number(min_val)), constrained_number(max_val));
}
} // namespace safe
