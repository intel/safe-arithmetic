#pragma once


#include <safe/detail/fwd.hpp>
#include <safe/detail/integral_type.hpp>
#include <safe/detail/assume.hpp>
#include <safe/detail/pure.hpp>
#include <safe/detail/var_assign_static_assert.hpp>
#include <safe/detail/concepts.hpp>
#include <safe/dsl.hpp>
#include <safe/dsl/simplify.hpp>

#include <limits>
#include <type_traits>
#include <compare>
#include <functional>
#include <bit>


namespace safe {
    template<typename T, auto Requirement>
    struct var {
    public:
        constexpr static auto requirement = Requirement;

        constexpr var() requires(requirement >= ival<0, 0>)
            : value_(0)
        {}

        template<typename U>
        requires (std::is_convertible_v<U, T>)
        SAFE_INLINE constexpr var(unsafe_cast_ferry<U> ferry)
            : value_{static_cast<T>(ferry.value())}
        {
            SAFE_ASSUME(requirement.check(value_));
        }

        SAFE_INLINE constexpr var(Var auto const & rhs)
            : value_(rhs.unsafe_value()) // intentionally allowing narrowing conversions
        {
            static_assert_assign_requirements(*this, rhs);
        }

        SAFE_INLINE constexpr var(Var auto && rhs)
            : value_(rhs.unsafe_value()) // intentionally allowing narrowing conversions
        {
            static_assert_assign_requirements(*this, rhs);
        }

        SAFE_INLINE constexpr auto operator=(Var auto const & rhs) -> var & {
            static_assert_assign_requirements(*this, rhs);
            value_ = rhs.unsafe_value();
            return *this;
        }

        SAFE_INLINE constexpr auto operator=(Var auto && rhs) -> var & {
            static_assert_assign_requirements(*this, rhs);
            value_ = rhs.unsafe_value();
            return *this;
        }

        [[nodiscard]] SAFE_INLINE constexpr T unsafe_value() const {
            SAFE_ASSUME(requirement.check(value_));
            return value_;
        }

    private:
        using primitive_contract = detail::integral_type<T>;

        static_assert(rhs_must_be_subset_of_lhs<
            lhs_req<decltype(primitive_contract::requirement)>,
            rhs_req<decltype(Requirement)>
        >::value);

        T value_;
    };

    namespace detail {
        [[nodiscard]] SAFE_INLINE constexpr auto bin_op(
            auto op,
            Var auto lhs,
            Var auto rhs
        ) {
            auto result_req =
                dsl::detail::simp(op(lhs.requirement, rhs.requirement));

            // FIXME: replace with embiggening strategy; the result type is
            //        always large enough to contain its value

            auto result = op(
                lhs.unsafe_value(),
                rhs.unsafe_value());

            return unsafe_cast<var<decltype(result), result_req>>(result);
        }
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator+(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::plus<>(), lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator-(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::minus<>(), lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator-(Var auto v) {
        constexpr auto zero = var<decltype(v.unsafe_value()), ival<0, 0>>{};
        return zero - v;
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator*(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::multiplies<>(), lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator/(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::divides<>(), lhs, rhs);
    }
    
    [[nodiscard]] SAFE_INLINE constexpr auto operator%(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::modulus<>(), lhs, rhs);
    }
    
    [[nodiscard]] SAFE_INLINE constexpr auto operator<<(Var auto lhs, Var auto rhs) {
        return detail::bin_op([](auto a, auto b){return a << b;}, lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator>>(Var auto lhs, Var auto rhs) {
        return detail::bin_op([](auto a, auto b){return a >> b;}, lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator|(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::bit_or<>(), lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator&(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::bit_and<>(), lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator^(Var auto lhs, Var auto rhs) {
        return detail::bin_op(std::bit_xor<>(), lhs, rhs);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator<=>(
        Var auto lhs,
        Var auto rhs
    )
        -> std::compare_three_way_result_t<decltype(lhs.unsafe_value()), decltype(rhs.unsafe_value())>
    {
        return lhs.unsafe_value() <=> rhs.unsafe_value();
    }

    [[nodiscard]] SAFE_INLINE constexpr auto operator==(Var auto lhs, Var auto rhs) -> bool {
        return lhs.unsafe_value() == rhs.unsafe_value();
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
        auto result_req = dsl::detail::simp(safe::dsl::bit_width(value.requirement));
        return unsafe_cast<var<num_t, result_req>>(result);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto min(
        Var auto lhs,
        Var auto rhs
    ) {
        using common_type = std::common_type_t<decltype(lhs.unsafe_value()), decltype(rhs.unsafe_value())>;
        auto result = std::min<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
        using result_t = decltype(result);
        auto result_req = dsl::detail::simp(dsl::min(lhs.requirement, rhs.requirement));
        return unsafe_cast<var<result_t, result_req>>(result);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto max(
        Var auto lhs,
        Var auto rhs
    ) {
        using common_type = std::common_type_t<decltype(lhs.unsafe_value()), decltype(rhs.unsafe_value())>;
        auto result = std::max<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
        using result_t = decltype(result);
        auto result_req = dsl::detail::simp(dsl::max(lhs.requirement, rhs.requirement));
        return unsafe_cast<var<result_t, result_req>>(result);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto clamp(
        Var auto value,
        Var auto min_val,
        Var auto max_val
    ) {
        return min(max(value, min_val), max_val);
    }

    [[nodiscard]] SAFE_INLINE constexpr auto clamp(
        auto unsafe_value,
        Var auto min_val,
        Var auto max_val
    ) {
        return clamp(value(unsafe_value), min_val, max_val);
    }
}

