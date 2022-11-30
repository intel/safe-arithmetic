#pragma once


#include <safe/detail/integral_type.hpp>

#include <safe/var.hpp>
#include <safe/constant.hpp>
#include <safe/dsl.hpp>

#include <cstdint>
#include <limits>


namespace safe {
    namespace int_types {
        template<int8_t value>
        constexpr auto s8_ = constant<int8_t, value>;
        using s8 = detail::integral_type<int8_t>;


        template<uint8_t value>
        constexpr auto u8_ = constant<uint8_t, value>;
        using u8 = detail::integral_type<uint8_t>;


        template<int16_t value>
        constexpr auto s16_ = constant<int16_t, value>;
        using s16 = detail::integral_type<int16_t>;


        template<uint16_t value>
        constexpr auto u16_ = constant<uint16_t, value>;
        using u16 = detail::integral_type<uint16_t>;

        
        template<uint32_t value>
        constexpr auto u32_ = constant<uint32_t, value>;
        using u32 = detail::integral_type<uint32_t>;


        template<int32_t value>
        constexpr auto s32_ = constant<int32_t, value>;
        using s32 = detail::integral_type<int32_t>;


        template<uint64_t value>
        constexpr auto u64_ = constant<uint64_t, value>;
        using u64 = detail::integral_type<uint64_t>;


        template<int64_t value>
        constexpr auto s64_ = constant<int64_t, value>;
        using s64 = detail::integral_type<int64_t>;
    }

    using namespace int_types;


    namespace interval_types {
        template<auto min, auto max>
        using ival_s8 = var<int8_t, ival<min, max>>;

        template<auto min, auto max>
        using ival_u8 = var<uint8_t, ival<min, max>>;
        
        template<auto min, auto max>
        using ival_s16 = var<int16_t, ival<min, max>>;

        template<auto min, auto max>
        using ival_u16 = var<uint16_t, ival<min, max>>;
        
        template<auto min, auto max>
        using ival_s32 = var<int32_t, ival<min, max>>;
        
        template<auto min, auto max>
        using ival_u32 = var<uint32_t, ival<min, max>>;

        template<auto min, auto max>
        using ival_s64 = var<int64_t, ival<min, max>>;

        template<auto min, auto max>
        using ival_u64 = var<uint64_t, ival<min, max>>;
    }

    using namespace interval_types;

    namespace detail {
        template<
            typename T,
            char... Chars>
        [[nodiscard]] constexpr auto to_constant() {
             constexpr T value = [](){
                constexpr std::array<char, sizeof...(Chars)> chars{Chars...};
                T sum = 0;

                for (char c : chars) {
                    T const digit = c - '0';
                    sum = (sum * 10) + digit;
                }

                return sum;
            }();

            return make_constant<T, value>();
        }
    }

    namespace literals {
        template<char... Chars>
        constexpr auto operator ""_s8() {
            return detail::to_constant<int8_t, Chars...>();
        }

        template<char... Chars>
        constexpr auto operator ""_u8() {
            return detail::to_constant<uint8_t, Chars...>();
        }
        
        template<char... Chars>
        constexpr auto operator ""_s16() {
            return detail::to_constant<int16_t, Chars...>();
        }

        template<char... Chars>
        constexpr auto operator ""_u16() {
            return detail::to_constant<uint16_t, Chars...>();
        }
        
        template<char... Chars>
        constexpr auto operator ""_s32() {
            return detail::to_constant<int32_t, Chars...>();
        }
            
        template<char... Chars>
        constexpr auto operator ""_u32() {
            return detail::to_constant<uint32_t, Chars...>();
        }
        
        template<char... Chars>
        constexpr auto operator ""_s64() {
            return detail::to_constant<int64_t, Chars...>();
        }
            
        template<char... Chars>
        constexpr auto operator ""_u64() {
            return detail::to_constant<uint64_t, Chars...>();
        }
    }
}