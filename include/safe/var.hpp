#pragma once


#include <safe/dsl.hpp>
#include <safe/dsl/simplify.hpp>

#include <limits>
#include <type_traits>
#include <compare>


namespace safe {
    template<typename U>
    [[nodiscard]] constexpr auto value(U value);

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

        template<
            typename U,
            auto OtherRequirement>
        friend constexpr auto std::abs(
            safe::var<U, OtherRequirement> const value
        );

        constexpr var(T value)
            : value_{value}
        {}

        template<
            typename RhsT,
            typename OpT>
        [[nodiscard]] constexpr auto bin_op(RhsT rhs, OpT op) const {
            auto result = op(value_, rhs.value_);
            using result_t = decltype(result);
            auto result_req = safe_dsl::detail::simp(op(requirement, rhs.requirement));
            return var<result_t, result_req>{result};
        }

    public:
        [[nodiscard]] constexpr T unsafe_value() const {
            return value_;
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        constexpr var(
            var<RhsT, RhsRequirement> rhs
        )
            // NOTE: not using the initializer list to allow for narrowing
            //       conversions. vars are expected to protect from
            //       overflows.
            : value_{}
        {
            static_assert(rhs_must_be_subset_of_lhs<lhs_req<decltype(Requirement)>, rhs_req<decltype(RhsRequirement)>>::value);
            value_ = rhs.value_;
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        constexpr auto operator=(
            var<RhsT, RhsRequirement> rhs
        ) {
            static_assert(rhs_must_be_subset_of_lhs<lhs_req<decltype(Requirement)>, rhs_req<decltype(RhsRequirement)>>::value);
            value_ = rhs.value_;
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator+(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a + b;});
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator-(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a - b;});
        }

        template<typename = std::enable_if_t<true, void>>
        [[nodiscard]] constexpr auto operator-() const {
            constexpr var<T, ival<0, 0>> zero{0};
            return zero - *this;
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator*(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a * b;});
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator/(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a / b;});
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator%(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a % b;});
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator<<(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a << b;});
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr auto operator>>(
            var<RhsT, RhsRequirement> const rhs
        ) const {
            return bin_op(rhs, [](auto a, auto b){return a >> b;});
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr std::strong_ordering operator<=>(
            var<RhsT, RhsRequirement> rhs
        ) const {
            return value_ <=> rhs.value_;
        }

        template<
            typename RhsT,
            auto RhsRequirement>
        [[nodiscard]] constexpr bool operator==(
            var<RhsT, RhsRequirement> rhs
        ) const {
            return value_ == rhs.value_;
        }
    };
}


namespace std {
    template<
        typename LhsT,
        auto LhsRequirement,
        typename RhsT,
        auto RhsRequirement,
        typename = std::enable_if_t<!std::is_same_v<safe::var<LhsT, LhsRequirement>, safe::var<RhsT, RhsRequirement>>, void>>
    [[nodiscard]] constexpr auto min(
        safe::var<LhsT, LhsRequirement> const lhs,
        safe::var<RhsT, RhsRequirement> const rhs
    ) {
        using common_type = std::common_type_t<LhsT, RhsT>;
        auto result = std::min<common_type>(lhs.value_, rhs.value_);
        using result_t = decltype(result);
        auto result_req = safe_dsl::detail::simp(safe_dsl::min(lhs.requirement, rhs.requirement));
        return safe::var<result_t, result_req>{result};
    }

    template<
        typename LhsT,
        auto LhsRequirement,
        typename RhsT,
        auto RhsRequirement,
        typename = std::enable_if_t<!std::is_same_v<safe::var<LhsT, LhsRequirement>, safe::var<RhsT, RhsRequirement>>, void>>
    [[nodiscard]] constexpr auto max(
        safe::var<LhsT, LhsRequirement> const lhs,
        safe::var<RhsT, RhsRequirement> const rhs
    ) {
        using common_type = std::common_type_t<LhsT, RhsT>;
        auto result = std::max<common_type>(lhs.value_, rhs.value_);
        using result_t = decltype(result);
        auto result_req = safe_dsl::detail::simp(safe_dsl::max(lhs.requirement, rhs.requirement));
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
    [[nodiscard]] constexpr auto clamp(
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
    [[nodiscard]] constexpr auto clamp(
        ValueT const unsafe_value,
        safe::var<MinT, MinRequirement> const min_val,
        safe::var<MaxT, MaxRequirement> const max_val
    ) {
        return std::clamp(safe::value(unsafe_value), min_val, max_val);
    }

    template<
        typename U,
        auto OtherRequirement>
    [[nodiscard]] constexpr auto abs(
        safe::var<U, OtherRequirement> const value
    ) {
        auto result = std::abs(value.value_);
        auto result_req = safe_dsl::detail::simp(safe_dsl::abs(value.requirement));
        return safe::var<U, result_req>{result};
    }
}