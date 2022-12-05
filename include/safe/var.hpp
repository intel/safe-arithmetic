#pragma once


#include <safe/detail/fwd.hpp>
#include <safe/detail/integral_type.hpp>
#include <safe/detail/assume.hpp>
#include <safe/detail/pure.hpp>
#include <safe/detail/var_assign_static_assert.hpp>
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
        constexpr static auto requirement = Requirement;

    private:
        using PrimitiveContract = detail::integral_type<T>;
        static_assert(rhs_must_be_subset_of_lhs<lhs_req<decltype(PrimitiveContract::requirement)>, rhs_req<decltype(Requirement)>>::value);

        T value_;

        template<typename U, auto OtherRequirement>
        friend struct var;

        template<typename U, U value>
        friend inline constexpr auto detail::make_constant();

        friend constexpr auto value(auto);

        template<typename Callable, typename... ArgTs>
        friend constexpr bool invoke(Callable, ArgTs...);

        template<typename RetT>
        friend inline constexpr auto function(auto, auto...);

        template<size_t max_iter>
        friend inline constexpr auto accumulate(auto, auto);

        friend inline constexpr auto operator+(Var auto, Var auto);
        friend inline constexpr auto operator-(Var auto, Var auto);
        friend inline constexpr auto operator-(Var auto);
        friend inline constexpr auto operator*(Var auto, Var auto);
        friend inline constexpr auto operator/(Var auto, Var auto);
        friend inline constexpr auto operator%(Var auto, Var auto);
        friend inline constexpr auto operator<<(Var auto, Var auto);
        friend inline constexpr auto operator>>(Var auto, Var auto);
        friend inline constexpr auto operator|(Var auto, Var auto);
        friend inline constexpr auto operator&(Var auto, Var auto);
        friend inline constexpr auto operator^(Var auto, Var auto);
        friend inline constexpr auto min(Var auto, Var auto);
        friend inline constexpr auto max(Var auto, Var auto);
        friend inline constexpr auto abs(Var auto);

        friend inline constexpr auto bit_width(Var auto);

        inline constexpr var(T value)
            : value_{value}
        {
            SAFE_ASSUME(requirement.check(value_));
        }

        [[nodiscard]] inline constexpr auto bin_op(
            Var auto rhs,
            auto op
        ) const {
            using num_t =
                std::common_type_t<T, decltype(rhs.unsafe_value())>;

            auto const result = static_cast<num_t>(op(
                static_cast<num_t>(unsafe_value()),
                static_cast<num_t>(rhs.unsafe_value())));

            auto result_req =
                safe::dsl::detail::simp(op(requirement, rhs.requirement));

            return var<num_t, result_req>{result};
        }

    public:
        constexpr var() = delete;

        inline constexpr var(Var auto const & rhs)
            : value_(rhs.unsafe_value()) // intentionally allowing narrowing conversions
        {
            static_assert_assign_requirements(*this, rhs);
        }

        inline constexpr var(Var auto && rhs)
            : value_(rhs.unsafe_value()) // intentionally allowing narrowing conversions
        {
            static_assert_assign_requirements(*this, rhs);
        }

        inline constexpr auto operator=(Var auto const & rhs) -> var & {
            static_assert_assign_requirements(*this, rhs);
            value_ = rhs.unsafe_value();
            return *this;
        }

        inline constexpr auto operator=(Var auto && rhs) -> var & {
            static_assert_assign_requirements(*this, rhs);
            value_ = rhs.unsafe_value();
            return *this;
        }

        [[nodiscard]] inline constexpr T unsafe_value() const {
            SAFE_ASSUME(requirement.check(value_));
            return value_;
        }
    };

    [[nodiscard]] inline constexpr auto operator+(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::plus<>());
    }

    [[nodiscard]] inline constexpr auto operator-(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::minus<>());
    }

    [[nodiscard]] inline constexpr auto operator-(Var auto v) {
        constexpr var<decltype(v.unsafe_value()), ival<0, 0>> zero{0};
        return zero - v;
    }

    [[nodiscard]] inline constexpr auto operator*(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::multiplies<>());
    }

    [[nodiscard]] inline constexpr auto operator/(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::divides<>());
    }
    
    [[nodiscard]] inline constexpr auto operator%(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::modulus<>());
    }
    
    [[nodiscard]] inline constexpr auto operator<<(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, [](auto a, auto b){return a << b;});
    }

    [[nodiscard]] inline constexpr auto operator>>(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, [](auto a, auto b){return a >> b;});
    }

    [[nodiscard]] inline constexpr auto operator|(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::bit_or<>());
    }

    [[nodiscard]] inline constexpr auto operator&(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::bit_and<>());
    }

    [[nodiscard]] inline constexpr auto operator^(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::bit_xor<>());
    }

    [[nodiscard]] inline constexpr auto operator<=>(
        Var auto lhs,
        Var auto rhs
    )
        -> std::compare_three_way_result_t<decltype(lhs.unsafe_value()), decltype(rhs.unsafe_value())>
    {
        return lhs.unsafe_value() <=> rhs.unsafe_value();
    }

    [[nodiscard]] inline constexpr auto operator==(Var auto lhs, Var auto rhs) -> bool {
        return lhs.unsafe_value() == rhs.unsafe_value();
    }

    [[nodiscard]] inline constexpr auto abs(Var auto value) {
        using num_t = decltype(value.unsafe_value());
        auto result = static_cast<num_t>(std::abs(value.unsafe_value()));
        auto result_req = dsl::detail::simp(safe::dsl::abs(value.requirement));
        return var<num_t, result_req>{result};
    }


    [[nodiscard]] inline constexpr auto bit_width(Var auto value) {
        using num_t = decltype(value.unsafe_value());
        auto result = static_cast<num_t>(std::bit_width(value.unsafe_value()));
        auto result_req = dsl::detail::simp(safe::dsl::bit_width(value.requirement));
        return var<num_t, result_req>{result};
    }

    [[nodiscard]] inline constexpr auto min(
        Var auto lhs,
        Var auto rhs
    ) {
        using common_type = std::common_type_t<decltype(lhs.unsafe_value()), decltype(rhs.unsafe_value())>;
        auto result = std::min<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
        using result_t = decltype(result);
        auto result_req = safe::dsl::detail::simp(safe::dsl::min(lhs.requirement, rhs.requirement));
        return safe::var<result_t, result_req>{result};
    }

    [[nodiscard]] inline constexpr auto max(
        Var auto lhs,
        Var auto rhs
    ) {
        using common_type = std::common_type_t<decltype(lhs.unsafe_value()), decltype(rhs.unsafe_value())>;
        auto result = std::max<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
        using result_t = decltype(result);
        auto result_req = safe::dsl::detail::simp(safe::dsl::max(lhs.requirement, rhs.requirement));
        return safe::var<result_t, result_req>{result};
    }

    [[nodiscard]] inline constexpr auto clamp(
        Var auto value,
        Var auto min_val,
        Var auto max_val
    ) {
        return min(max(value, min_val), max_val);
    }

    [[nodiscard]] inline constexpr auto clamp(
        auto unsafe_value,
        Var auto min_val,
        Var auto max_val
    ) {
        return clamp(value(unsafe_value), min_val, max_val);
    }
}

