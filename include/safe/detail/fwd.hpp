#pragma once


#include <type_traits>


#ifndef SAFE_INLINE
#define SAFE_INLINE inline
#endif

namespace safe {
    template<typename T, auto Requirement>
    struct var;

    template <typename T>
    constexpr bool is_var_v = false;

    template<typename T, auto Requirement>
    constexpr bool is_var_v<var<T, Requirement>> = true;

    template<typename T>
    concept Var = is_var_v<T>;


    template<std::size_t NumBits>
    struct big_integer;

    template <typename T>
    constexpr bool is_big_integer_v = false;

    template<std::size_t NumBits>
    constexpr bool is_big_integer_v<big_integer<NumBits>> = true;

    template<typename T>
    concept BigInteger = is_big_integer_v<T>;


    template <typename T>
    constexpr bool is_big_integral_constant_v = false;

    template<std::size_t NumBits, big_integer<NumBits> Value>
    constexpr bool is_big_integral_constant_v<std::integral_constant<big_integer<NumBits>, Value>> = true;

    template<typename T>
    concept BigIntegralConstant = is_big_integral_constant_v<T>;

    template<typename T>
    concept ConvertableToBigInteger = is_big_integer_v<T> || is_big_integral_constant_v<T>;

    template<typename T>
    struct as_big_integer;

    template<std::size_t NumBits>
    struct as_big_integer<big_integer<NumBits>> {
        using type = big_integer<NumBits>;
    };

    template<std::size_t NumBits, big_integer<NumBits> Value>
    struct as_big_integer<std::integral_constant<big_integer<NumBits>, Value>> {
        using type = big_integer<NumBits>;
    };

    template<typename T>
    using as_big_integer_t = typename as_big_integer<T>::type;

    [[nodiscard]] inline constexpr auto value(auto value);

    namespace detail {
        template<typename U, U value>
        [[nodiscard]] inline constexpr auto make_constant();
    }

    template<typename T>
    struct unsafe_cast_ferry {
    private:
        T v;

    public:
        SAFE_INLINE constexpr explicit(true) unsafe_cast_ferry(
            T new_value
        )
            : v{new_value}
        {}

        [[nodiscard]] SAFE_INLINE constexpr T value() const {
            return v;
        }
    };

    template<typename T>
    requires(Var<T> || BigInteger<T>)
    [[nodiscard]] constexpr auto unsafe_cast(auto const & src) {
        return T{unsafe_cast_ferry{src}};
    }

    template<typename T>
    requires(!Var<T> && !BigInteger<T>)
    [[nodiscard]] constexpr auto unsafe_cast(auto const & src) {
        return src;
    }
}