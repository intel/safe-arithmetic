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

    template<typename T>
    struct is_var : public std::integral_constant<bool, false> {};

    template<typename T, auto Requirement>
    struct is_var<var<T, Requirement>> : public std::integral_constant<bool, true> {};

    template<typename T>
    constexpr bool is_var_v = is_var<T>{};

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

        template<
            typename LhsT,
            auto LhsRequirement,
            typename RhsT,
            auto RhsRequirement,
            typename>
        friend constexpr auto std::min(
            safe::var<LhsT, LhsRequirement> const lhs,
            safe::var<RhsT, RhsRequirement> const rhs
        );

        template<
            typename LhsT,
            auto LhsRequirement,
            typename RhsT,
            auto RhsRequirement,
            typename>
        friend constexpr auto std::max(
            safe::var<LhsT, LhsRequirement> const lhs,
            safe::var<RhsT, RhsRequirement> const rhs
        );

        friend constexpr auto std::abs(
            Var auto value
        );

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

        template<typename = std::enable_if_t<true, void>>
        [[nodiscard]] inline constexpr auto operator-() const {
            constexpr var<T, ival<0, 0>> zero{0};
            return zero - *this;
        }

        friend inline constexpr auto operator+(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator-(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator*(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator/(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator%(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator<<(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator>>(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator|(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator&(Var auto lhs, Var auto rhs);
        friend inline constexpr auto operator^(Var auto lhs, Var auto rhs);
    };

    [[nodiscard]] inline constexpr auto operator+(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::plus<>());
    }

    [[nodiscard]] inline constexpr auto operator-(Var auto lhs, Var auto rhs) {
        return lhs.bin_op(rhs, std::minus<>());
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
}


namespace std {
    template<
        typename LhsT,
        auto LhsRequirement,
        typename RhsT,
        auto RhsRequirement,
        typename = std::enable_if_t<!std::is_same_v<safe::var<LhsT, LhsRequirement>, safe::var<RhsT, RhsRequirement>>, void>>
    [[nodiscard]] inline constexpr auto min(
        safe::var<LhsT, LhsRequirement> const lhs,
        safe::var<RhsT, RhsRequirement> const rhs
    ) {
        using common_type = std::common_type_t<LhsT, RhsT>;
        auto result = std::min<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
        using result_t = decltype(result);
        auto result_req = safe::dsl::detail::simp(safe::dsl::min(lhs.requirement, rhs.requirement));
        return safe::var<result_t, result_req>{result};
    }

    template<
        typename LhsT,
        auto LhsRequirement,
        typename RhsT,
        auto RhsRequirement,
        typename = std::enable_if_t<!std::is_same_v<safe::var<LhsT, LhsRequirement>, safe::var<RhsT, RhsRequirement>>, void>>
    [[nodiscard]] inline constexpr auto max(
        safe::var<LhsT, LhsRequirement> const lhs,
        safe::var<RhsT, RhsRequirement> const rhs
    ) {
        using common_type = std::common_type_t<LhsT, RhsT>;
        auto result = std::max<common_type>(lhs.unsafe_value(), rhs.unsafe_value());
        using result_t = decltype(result);
        auto result_req = safe::dsl::detail::simp(safe::dsl::max(lhs.requirement, rhs.requirement));
        return safe::var<result_t, result_req>{result};
    }

    template<
        typename ValueT,
        auto ValueRequirement,
        typename MinT,
        auto MinRequirement,
        typename MaxT,
        auto MaxRequirement,
        typename =
            std::enable_if_t<
                (
                    !std::is_same_v<
                        safe::var<ValueT, ValueRequirement>,
                        safe::var<MinT, MinRequirement>
                    >
                    ||
                    !std::is_same_v<
                        safe::var<ValueT, ValueRequirement>,
                        safe::var<MaxT, MaxRequirement>
                    >
                    ||
                    !std::is_same_v<
                        safe::var<MinT, MinRequirement>,
                        safe::var<MaxT, MaxRequirement>
                    >
                ),
                void
            >
    >
    [[nodiscard]] inline constexpr auto clamp(
        safe::var<ValueT, ValueRequirement> const value,
        safe::var<MinT, MinRequirement> const min_val,
        safe::var<MaxT, MaxRequirement> const max_val
    ) {
        return std::min(std::max(value, min_val), max_val);
    }

    template<
        typename ValueT,
        typename MinT,
        auto MinRequirement,
        typename MaxT,
        auto MaxRequirement,
        typename =
            std::enable_if_t<
                (
                    !std::is_same_v<
                        ValueT,
                        safe::var<MinT, MinRequirement>
                    >
                    ||
                    !std::is_same_v<
                        ValueT,
                        safe::var<MaxT, MaxRequirement>
                    >
                    ||
                    !std::is_same_v<
                        safe::var<MinT, MinRequirement>,
                        safe::var<MaxT, MaxRequirement>
                    >
                ),
                void
            >
    >
    [[nodiscard]] inline constexpr auto clamp(
        ValueT const unsafe_value,
        safe::var<MinT, MinRequirement> const min_val,
        safe::var<MaxT, MaxRequirement> const max_val
    ) {
        return std::clamp(safe::value(unsafe_value), min_val, max_val);
    }

    [[nodiscard]] inline constexpr auto abs(
        safe::Var auto value
    ) {
        using num_t = decltype(value.unsafe_value());
        auto result = static_cast<num_t>(std::abs(value.unsafe_value()));
        auto result_req = safe::dsl::detail::simp(safe::dsl::abs(value.requirement));
        return safe::var<num_t, result_req>{result};
    }
}