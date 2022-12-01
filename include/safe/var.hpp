#pragma once

#include <safe/detail/assume.hpp>
#include <safe/detail/pure.hpp>
#include <safe/dsl.hpp>
#include <safe/dsl/simplify.hpp>

#include <limits>
#include <type_traits>
#include <compare>
#include <functional>


namespace safe {
    template<typename U>
    [[nodiscard]] inline constexpr auto value(U value);

    template<typename U>
    struct lhs_req {
        constexpr static U value{};
    };

    template<typename U>
    struct rhs_req {
        constexpr static U value{};
    };

    template<typename LhsT, typename RhsT>
    struct rhs_must_be_subset_of_lhs {
        constexpr static bool value = LhsT::value >= RhsT::value;
    };


    template<typename T, auto Requirement>
    struct var;

    template <typename T>
    constexpr bool is_var_v = false;

    template<typename T, auto Requirement>
    constexpr bool is_var_v<var<T, Requirement>> = true;

    template<class T>
    concept Var = is_var_v<T>;


    namespace detail {
        template<typename T>
        using integral_type =
            var<T, ival<
                std::numeric_limits<T>::lowest(),
                std::numeric_limits<T>::max()>
            >;
    }

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
        friend constexpr auto make_constant();

        template<typename U>
        friend constexpr auto value(U value);

        template<typename Callable, typename... ArgTs>
        friend constexpr bool invoke(Callable c, ArgTs... args);

        friend inline constexpr auto operator+(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator-(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator-(Var auto v);
        friend inline constexpr auto operator*(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator/(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator%(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator<<(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator>>(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator|(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator&(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator^(Var auto lhs, Var auto rhs);
        friend inline constexpr auto min(Var auto lhs, Var auto rhs);
        friend inline constexpr auto max(Var auto lhs, Var auto rhs);
        friend inline constexpr auto abs(Var auto value);

        constexpr var(T value)
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
        inline constexpr var(
            Var auto rhs
        )
            // NOTE: not using the initializer list to allow for narrowing
            //       conversions. vars are expected to protect from
            //       overflows.
            : value_{}
        {
            static_assert(rhs_must_be_subset_of_lhs<lhs_req<decltype(Requirement)>, rhs_req<decltype(rhs.requirement)>>::value);
            value_ = rhs.unsafe_value();
        }

        inline constexpr auto operator=(Var auto rhs) -> var & {
            static_assert(rhs_must_be_subset_of_lhs<lhs_req<decltype(Requirement)>, rhs_req<decltype(rhs.requirement)>>::value);
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

    [[nodiscard]] inline constexpr std::strong_ordering operator<=>(Var auto lhs, Var auto rhs) {
        return lhs.unsafe_value() <=> rhs.unsafe_value();
    }

    [[nodiscard]] inline constexpr bool operator==(Var auto lhs, Var auto rhs) {
        return lhs.unsafe_value() == rhs.unsafe_value();
    }

    [[nodiscard]] inline constexpr auto abs(
        Var auto value
    ) {
        using num_t = decltype(value.unsafe_value());
        auto result = static_cast<num_t>(std::abs(value.unsafe_value()));
        auto result_req = dsl::detail::simp(safe::dsl::abs(value.requirement));
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

