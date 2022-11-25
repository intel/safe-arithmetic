#pragma once

#include <safe/detail/pure.hpp>
#include <safe/dsl/ival.hpp>
#include <safe/dsl/triint.hpp>

#include <type_traits>

namespace safe::dsl {
    template<auto VariableBits, auto ConstantBits = 0u>
    struct mask_t {
        using type = mask_t;

        constexpr static auto var_bits = VariableBits;
        constexpr static auto const_bits = ConstantBits;
        constexpr static auto value = triint{var_bits, const_bits};

        [[nodiscard]] SAFE_PURE constexpr static bool check(auto value) {
            return
                (~var_bits & value) ==
                (~var_bits & const_bits);
        }
    };

    template<auto VariableBits, auto ConstantBits = 0u>
    constexpr mask_t<VariableBits, ConstantBits> mask{};

    namespace detail {
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
        struct to_ival {};

        template<auto var_bits, auto const_bits>
        struct to_ival<mask_t<var_bits, const_bits>> {
            /// lower bits that can change, including gaps between var_bits
            constexpr static auto var_portion = fill_in_bitmask(var_bits);

            /// fixed bits that will never change, excludes the var_portion
            constexpr static auto const_portion = const_bits & ~var_portion;

            constexpr static auto min = const_portion;
            constexpr static auto max = const_portion + (var_portion & var_bits);

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

    template<typename T>
    struct is_mask : public std::integral_constant<bool, false> {};

    template<auto var_bits, auto const_bits>
    struct is_mask<mask_t<var_bits, const_bits>> : public std::integral_constant<bool, true> {};

    template<typename T>
    constexpr bool is_mask_v = is_mask<T>{};

    template<class T>
    concept Mask = is_mask_v<T>;
}