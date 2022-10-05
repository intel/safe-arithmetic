#pragma once

#include <safe/detail/fwd.hpp>
#include <safe/constant.hpp>
#include <safe/check_and_trap.hpp>
#include <safe/check_and_clamp.hpp>

#include <boost/mp11.hpp>

#include <algorithm>
#include <array>


namespace safe {
    using namespace boost::mp11;


    template<typename T, typename... Requirements>
    struct contract {
    private:
        T value_;

        template<typename U, typename... OtherRequirements>
        friend struct contract;

        template<typename U, U min, U max>
        friend constexpr auto clamp(
            U value
        ) -> contract<U, interval<min, max>>;

        template<typename U, U value>
        friend constexpr auto make_constant();

        template<typename U>
        friend constexpr auto value(U value);

        template<typename Callable, typename... ArgTs>
        friend constexpr bool invoke(Callable c, ArgTs... args);


        constexpr contract(T value)
            : value_{value}
        {}

        template<typename OtherT, typename... OtherRequirements>
        constexpr static auto make_contract(
            OtherT value,
            mp_list<OtherRequirements...>
        ) -> contract<OtherT, OtherRequirements...> {
            return {value};
        }

        template<template<typename, typename> typename Operation, typename... RhsRequirements>
        constexpr auto binary_operation(
            T const value
        ) const {
            using operation_fn = binary_interval_operation_q<Operation>;
            using lhs_req = mp_list<Requirements...>;
            using rhs_req = mp_list<RhsRequirements...>;
            using result_reqs = mp_product_q<operation_fn, lhs_req, rhs_req>;

            return make_contract(value, result_reqs{});
        }


        template<typename Req1, typename Req2, typename... ReqN>
        constexpr T clamp_from_reqs(T value) const {
            return clamp_from_reqs<Req2, ReqN...>(Req1::clamp(value));
        }

        template<typename Req1>
        constexpr static T clamp_from_reqs(T value) {
            return Req1::clamp(value);
        }

        template<
            typename RhsT,
            typename... RhsRequirements>
        constexpr static RhsT check_and_clamp_impl(
           check_and_clamp<RhsT, RhsRequirements...> rhs
        ) {
            auto const unsafe_value = rhs.value_.unsafe_value();

            if constexpr (is_assignable_v<contract<RhsT, RhsRequirements...>>) {
                return unsafe_value;

            } else if (check(unsafe_value)) {
                return unsafe_value;

            } else {
                return clamp_from_reqs<RhsRequirements...>(unsafe_value);
            }
        }

        template<
            typename RhsT,
            typename... RhsRequirements>
        static RhsT check_and_trap_impl(
            check_and_trap<RhsT, RhsRequirements...> rhs
        ) {
            auto const unsafe_value = rhs.value_.unsafe_value();

            if constexpr (is_assignable_v<contract<RhsT, RhsRequirements...>>) {
                return unsafe_value;

            } else if (check(rhs.value_.unsafe_value())) {
                return unsafe_value;

            } else {
                // FIXME: trap
            }
        }

    public:
        constexpr T unsafe_value() const {
            return value_;
        }

        template<typename U>
        struct is_assignable : public std::integral_constant<bool, false> {};

        template<
            typename RhsT,
            typename... RhsRequirements>
        struct is_assignable<contract<RhsT, RhsRequirements...>> {
            using lhs_req = mp_list<Requirements...>;
            using rhs_req = mp_list<RhsRequirements...>;
            using req_x_check = mp_product<req_is_assignable, lhs_req, rhs_req>;
            using is_valid = mp_all_of<req_x_check, mp_to_bool>;
            constexpr static bool value = is_valid::value;
        };

        template<typename U>
        constexpr static bool is_assignable_v = is_assignable<U>::value;


        /// return true if value is compatible with this contract
        template<typename U>
        constexpr static auto check(U value) {
            if constexpr (is_assignable_v<U>) {
                return true;
            } else {
                return (Requirements::check(value) && ...);
            }
        }



        template<
            typename RhsT,
            typename... RhsRequirements>
        constexpr contract(
            contract<RhsT, RhsRequirements...> rhs
        )
            : value_{rhs.value_}
        {
            // TODO: need to describe why an assignment or copy constructor fails
            static_assert(is_assignable_v<contract<RhsT, RhsRequirements...>>);
        }

        template<
            typename RhsT,
            typename... RhsRequirements>
        contract(
            check_and_trap<RhsT, RhsRequirements...> rhs
        )
            : value_{check_and_trap_impl(rhs)}
        {}


        template<
            typename RhsT,
            typename... RhsRequirements>
        constexpr contract(
            check_and_clamp<RhsT, RhsRequirements...> rhs
        )
            : value_{check_and_clamp_impl(rhs)}
        {}

        template<
            typename RhsT,
            typename... RhsRequirements>
        constexpr auto operator=(
            contract<RhsT, RhsRequirements...> rhs
        ) {
            // TODO: need to describe why an assignment or copy constructor fails
            static_assert(is_assignable_v<contract<RhsT, RhsRequirements...>>);
            value_ = rhs.value_;
        }

        template<
            typename RhsT,
            typename... RhsRequirements>
        constexpr auto operator=(
            check_and_trap<RhsT, RhsRequirements...> rhs
        ) {
            value_ = check_and_trap_impl(rhs);
        }


        template<
            typename RhsT,
            typename... RhsRequirements>
        constexpr auto operator=(
            check_and_clamp<RhsT, RhsRequirements...> rhs
        ) {
            value_ = check_and_clamp_impl(rhs);
        }

        constexpr auto operator-() const {
            return safe::constant<T, 0> - *this;
        }

        template<typename... RhsRequirements>
        constexpr auto operator+(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_plus, RhsRequirements...>(value_ + rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr auto operator-(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_minus, RhsRequirements...>(value_ - rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr auto operator*(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_multiply, RhsRequirements...>(value_ * rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr auto operator/(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_divide, RhsRequirements...>(value_ / rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr auto operator%(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_modulo, RhsRequirements...>(value_ % rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr auto operator<<(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_shift_left, RhsRequirements...>(value_ << rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr auto operator>>(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return binary_operation<req_operator_shift_right, RhsRequirements...>(value_ >> rhs.value_);
        }

        template<typename... RhsRequirements>
        constexpr bool operator<(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return value_ < rhs.value_;
        }

        template<typename... RhsRequirements>
        constexpr bool operator<=(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return value_ <= rhs.value_;
        }

        template<typename... RhsRequirements>
        constexpr bool operator==(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return value_ == rhs.value_;
        }

        template<typename... RhsRequirements>
        constexpr bool operator!=(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return value_ != rhs.value_;
        }

        template<typename... RhsRequirements>
        constexpr bool operator>=(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return value_ >= rhs.value_;
        }

        template<typename... RhsRequirements>
        constexpr bool operator>(
            contract<T, RhsRequirements...> const rhs
        ) const {
            return value_ > rhs.value_;
        }
    };

}







