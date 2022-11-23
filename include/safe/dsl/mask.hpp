#pragma once

#include <safe/dsl/ival.hpp>

#include <type_traits>

namespace safe::dsl {
    template<auto VariableBits, auto ConstantBits = 0u>
    struct mask_t {
        using type = mask_t;

        constexpr static auto var_bits = VariableBits;
        constexpr static auto const_bits = ConstantBits;


        [[nodiscard]] constexpr static bool check(auto value) {
            return (~var_bits & value) == const_bits;
        }
    };

    template<auto VariableBits, auto ConstantBits = 0u>
    constexpr mask_t<VariableBits, ConstantBits> mask{};

    namespace detail {
        template<typename T>
        struct to_ival {};

        template<auto var_bits, auto const_bits>
        struct to_ival<mask_t<var_bits, const_bits>> {
            constexpr static auto min = const_bits & ~var_bits;
            constexpr static auto max = min + var_bits;
            using type = ival_t<min, max>;
        };

        template<auto min, auto max>
        struct to_ival<ival_t<min, max>> {
            using type = ival_t<min, max>;
        };

        template<typename T>
        using to_ival_t = typename to_ival<T>::type;

        [[nodiscard]] constexpr bool is_basic_mask(auto value) {
            return ((value >> 1) & value) == (value >> 1);
        }

        /**
         * Takes a bitmask and sets all digits to the right of the first
         * '1' to '1'.
         */
        [[nodiscard]] constexpr auto fill_in_bitmask(auto value) {
            auto prev_value = value;

            do {
                prev_value = value;
                value |= (value >> 1);
            } while (prev_value != value);

            return value;
        }
        
        template<typename T>
        struct to_mask {};

        template<auto var_bits, auto const_bits>
        struct to_mask<mask_t<var_bits, const_bits>> {
            using type = mask_t<var_bits, const_bits>;
        };

        template<auto min, auto max>
        struct to_mask<ival_t<min, max>> {
            constexpr static auto var_bits = fill_in_bitmask(max ^ min);
            constexpr static auto const_bits = min & ~var_bits;
            using type = mask_t<var_bits, const_bits>;
        };


        template<typename T>
        using to_mask_t = typename to_mask<T>::type;
    }
}