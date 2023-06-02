#pragma once


#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/algorithms.hpp>
#include <safe/big_integer/detail/bitwise.hpp>

#include <functional>
#include <concepts>

// TODO: can we consolidate the implementations for left and right shift?
namespace safe::_big_integer::detail {
    template<typename T>
    struct shift_left {
        T source;
        int32_t shift_amt;

        constexpr shift_left(
            T source_arg, 
            int32_t shift_amt_arg
        ) 
            : source{source_arg}
            , shift_amt{shift_amt_arg}
        {}

        [[nodiscard]] constexpr auto get(int32_t i) const -> elem_t {
            return source.get(i - shift_amt);
        }
    };

    constexpr static auto bit_shift_left = [](
        auto & result,
        auto const & lhs,
        std::signed_integral auto const rhs
    ) -> void {
        auto const elem_shift_amt = rhs / 32;
        auto const bit_shift_amt = rhs % 32;

        auto const lhs_shifted_upper = shift_left(lhs, elem_shift_amt);
        auto const lhs_shifted_lower = shift_left(lhs, elem_shift_amt + 1);

        reverse_zip_transform([=](
            elem_t const upper,
            elem_t const lower
        ){
            if (bit_shift_amt == 0) {
                return upper;
            } else {
                return
                    (upper << bit_shift_amt) |
                    (lower >> (32 - bit_shift_amt));
            }

        })(result, lhs_shifted_upper, lhs_shifted_lower);
    };

    template<typename T>
    struct shift_right {
        T source;
        int32_t shift_amt;

        constexpr shift_right(
            T source_arg, 
            int32_t shift_amt_arg
        ) 
            : source{source_arg}
            , shift_amt{shift_amt_arg}
        {}

        [[nodiscard]] constexpr auto get(int32_t i) const -> elem_t {
            return source.get(i + shift_amt);
        }
    };

    constexpr static auto bit_shift_right = [](
        auto & result,
        auto const & lhs,
        std::signed_integral auto const rhs
    ) -> void {
        auto const elem_shift_amt = rhs / 32;
        auto const bit_shift_amt = rhs % 32;

        auto const lhs_shifted_upper = shift_right(lhs, elem_shift_amt + 1);
        auto const lhs_shifted_lower = shift_right(lhs, elem_shift_amt);

        zip_transform([=](
            elem_t const upper,
            elem_t const lower
        ){
            if (bit_shift_amt == 0) {
                return lower;
            } else {
                return
                    (upper << (32 - bit_shift_amt)) |
                    (lower >> bit_shift_amt);
            }

        })(result, lhs_shifted_upper, lhs_shifted_lower);
    };
}