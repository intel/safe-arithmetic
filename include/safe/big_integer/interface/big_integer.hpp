#pragma once


#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/operators.hpp>
#include <safe/big_integer/interface/fwd.hpp>

#include <cstdint>
#include <functional>
#include <cmath>


namespace safe::_big_integer::interface {
    template<std::size_t NumBits>
    struct big_integer {
        detail::storage<NumBits> unsafe_storage;

        constexpr big_integer(auto value) : unsafe_storage{detail::to_storage(value)} {}
        constexpr big_integer() : unsafe_storage{} {}

        constexpr auto operator&=(auto const & rhs) -> big_integer & {
            return do_assign_op(detail::bit_and, rhs);
        }

        constexpr auto operator|=(auto const & rhs) -> big_integer & {
            return do_assign_op(detail::bit_or, rhs);
        }

        constexpr auto operator^=(auto const & rhs) -> big_integer & {
            return do_assign_op(detail::bit_xor, rhs);
        }

        constexpr auto operator+=(auto const & rhs) -> big_integer & {
            return do_assign_op(detail::plus, rhs);
        }

        constexpr auto operator-=(auto const & rhs) -> big_integer & {
            return do_assign_op(detail::minus, rhs);
        }

        constexpr auto operator/=(auto const & raw_rhs) -> big_integer & {
            auto rhs = detail::to_storage(raw_rhs);

            decltype(unsafe_storage) q{};
            decltype(unsafe_storage) r{};

            detail::divmod(q, r, unsafe_storage, rhs);
            unsafe_storage = q;

            return *this;
        }

        constexpr auto operator%=(auto const & raw_rhs) -> big_integer & {
            auto rhs = detail::to_storage(raw_rhs);

            decltype(unsafe_storage) q{};
            decltype(unsafe_storage) r{};

            detail::divmod(q, r, unsafe_storage, rhs);
            unsafe_storage = r;

            return *this;
        }

        constexpr auto operator<<=(auto const & raw_rhs) -> big_integer & {
            auto rhs = detail::to_integral(raw_rhs);
            detail::bit_shift_left(unsafe_storage, unsafe_storage, rhs);
            return *this;
        }

        constexpr auto operator>>=(auto const & raw_rhs) -> big_integer & {
            auto rhs = detail::to_integral(raw_rhs);
            detail::bit_shift_right(unsafe_storage, unsafe_storage, rhs);
            return *this;
        }

        constexpr auto operator-() const -> big_integer<NumBits + 1> {
            big_integer<NumBits + 1> new_value{};
            detail::negate(new_value.unsafe_storage, unsafe_storage);
            return new_value;
        }

        constexpr auto operator~() const -> big_integer  {
            big_integer new_value{};
            detail::bit_not(new_value.unsafe_storage, unsafe_storage);
            return new_value;
        }

    private:
        constexpr auto do_assign_op(auto op, auto const & rhs) -> big_integer & {
            op(unsafe_storage, unsafe_storage, detail::to_storage(rhs));
            return *this;
        }
    };

    template<typename L, typename R>
    struct common_integer;

    template<std::size_t LhsNumBits, std::size_t RhsNumBits>
    struct common_integer<big_integer<LhsNumBits>, big_integer<RhsNumBits>> {
        using type = big_integer<std::max(LhsNumBits, RhsNumBits)>;
    };
    
    template<std::signed_integral T, std::size_t RhsNumBits>
    struct common_integer<T, big_integer<RhsNumBits>> {
        using type = big_integer<std::max<std::size_t>(sizeof(T) * 8, RhsNumBits)>;
    };
    
    template<std::unsigned_integral T, std::size_t RhsNumBits>
    struct common_integer<T, big_integer<RhsNumBits>> {
        using type = big_integer<std::max<std::size_t>((sizeof(T) * 8) + 1, RhsNumBits)>;
    };
    
    template<std::size_t LhsNumBits, std::signed_integral T>
    struct common_integer<big_integer<LhsNumBits>, T> {
        using type = big_integer<std::max<std::size_t>(sizeof(T) * 8, LhsNumBits)>;
    };
    
    template<std::size_t LhsNumBits, std::unsigned_integral T>
    struct common_integer<big_integer<LhsNumBits>, T> {
        using type = big_integer<std::max<std::size_t>((sizeof(T) * 8) + 1, LhsNumBits)>;
    };
    
    template<typename L, typename R>
    using common_integer_t = typename common_integer<std::remove_cvref_t<L>, std::remove_cvref_t<R>>::type;



    template<typename L, typename R>
    constexpr bool at_least_one_big_integer = false;

    template<std::size_t LhsNumBits, typename R>
    constexpr bool at_least_one_big_integer<big_integer<LhsNumBits>, R> = true;

    template<typename L, std::size_t RhsNumBits>
    constexpr bool at_least_one_big_integer<L, big_integer<RhsNumBits>> = true;

    template<std::size_t LhsNumBits, std::size_t RhsNumBits>
    constexpr bool at_least_one_big_integer<big_integer<LhsNumBits>, big_integer<RhsNumBits>> = true;

    template<std::size_t NumBits>
    big_integer(detail::storage<NumBits>) -> big_integer<NumBits>;


    [[nodiscard]] constexpr auto do_binary_op(
        auto op,
        auto width_calc,
        auto const & raw_lhs,
        auto const & raw_rhs
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        auto rhs = detail::to_storage(raw_rhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;
        using rhs_t = std::remove_cvref_t<decltype(rhs)>;

        constexpr auto result_width = width_calc(lhs_t::num_bits, rhs_t::num_bits);
        detail::storage<result_width> result{};

        op(result, lhs, rhs);

        return big_integer<result_width>(result);
    }

    template<std::size_t LhsNumBits, std::size_t RhsNumBits>
    [[nodiscard]] constexpr auto operator==(
        big_integer<LhsNumBits> const & lhs,
        big_integer<RhsNumBits> const & rhs
    ) -> bool {
        return lhs.unsafe_storage == rhs.unsafe_storage;
    }

    template<std::size_t LhsNumBits>
    [[nodiscard]] constexpr auto operator==(
        big_integer<LhsNumBits> const & lhs,
        std::integral auto const & rhs
    ) -> bool {
        return lhs.unsafe_storage == detail::to_storage(rhs);
    }
      
   
    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator<=>(
        L const & raw_lhs,
        R const & raw_rhs
    ) -> std::strong_ordering {
        auto lhs = detail::to_storage(raw_lhs);
        auto rhs = detail::to_storage(raw_rhs);
        return lhs <=> rhs;
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator+(
        L const & raw_lhs,
        R const & raw_rhs
    ) {
        return do_binary_op(detail::plus, detail::max_width_plus_one, raw_lhs, raw_rhs);
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator-(
        L const & raw_lhs,
        R const & raw_rhs
    ) {
        return do_binary_op(detail::minus, detail::max_width_plus_one, raw_lhs, raw_rhs);
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
   [[nodiscard]] constexpr auto operator*(
       L const & raw_lhs,
       R const & raw_rhs
   ) {
       return do_binary_op(detail::multiplies, detail::sum_width, raw_lhs, raw_rhs);
   }    
   
    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
   [[nodiscard]] constexpr auto divmod(
        L const & raw_lhs,
        R const & raw_rhs
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        auto rhs = detail::to_storage(raw_rhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;
        using rhs_t = std::remove_cvref_t<decltype(rhs)>;

        lhs_t q{};
        lhs_t r{};

        detail::divmod(q, r, lhs, rhs);

        return std::make_pair(big_integer{q}, big_integer{r});
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator/(
        L const & lhs,
        R const & rhs
    ) {
        auto [q, r] = divmod(lhs, rhs);
        return q;
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator%(
        L const & lhs,
        R const & rhs
    ) {
        auto [q, r] = divmod(lhs, rhs);
        return r;
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator|(
        L const & raw_lhs,
        R const & raw_rhs
    ) {
        return do_binary_op(detail::bit_or, detail::max_width, raw_lhs, raw_rhs);
    }

    [[nodiscard]] constexpr auto operator&(
        auto const & raw_lhs,
        auto const & raw_rhs
    ) {
        return do_binary_op(detail::bit_and, detail::max_width, raw_lhs, raw_rhs);
    }

    template<typename L, typename R>
    requires at_least_one_big_integer<L, R>
    [[nodiscard]] constexpr auto operator^(
        L const & raw_lhs,
        R const & raw_rhs
    ) {
        return do_binary_op(detail::bit_xor, detail::max_width, raw_lhs, raw_rhs);
    }

    [[nodiscard]] constexpr auto operator<<(
        auto const & raw_lhs,
        std::integral auto const & rhs
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;

        lhs_t result{};

        detail::bit_shift_left(result, lhs, rhs);

        return big_integer{result};
    }

    template<typename T, T rhs>
    [[nodiscard]] constexpr auto operator<<(
        auto const & raw_lhs,
        std::integral_constant<T, rhs>
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;

        using result_t = detail::storage<lhs_t::num_bits + rhs>;
        result_t result{};

        detail::bit_shift_left(result, lhs, rhs);

        return big_integer{result};
    }

    template<std::size_t NumBits>
    [[nodiscard]] constexpr auto operator<<(
        auto const & raw_lhs,
        big_integer<NumBits> raw_rhs
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;

        auto rhs = detail::to_integral(raw_rhs);

        lhs_t result{};
        
        detail::bit_shift_left(result, lhs, rhs);

        return big_integer{result};
    }


    [[nodiscard]] constexpr auto operator>>(
        auto const & raw_lhs,
        std::integral auto const & rhs
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;

        lhs_t result{};

        detail::bit_shift_right(result, lhs, rhs);

        return big_integer{result};
    }

    template<typename T, T rhs>
    [[nodiscard]] constexpr auto operator>>(
        auto const & raw_lhs,
        std::integral_constant<T, rhs>
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;

        using result_t = detail::storage<lhs_t::num_bits - rhs>;
        result_t result{};

        detail::bit_shift_right(result, lhs, rhs);

        return big_integer{result};
    }

    template<std::size_t NumBits>
    [[nodiscard]] constexpr auto operator>>(
        auto const & raw_lhs,
        big_integer<NumBits> raw_rhs
    ) {
        auto lhs = detail::to_storage(raw_lhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;

        auto rhs = detail::to_integral(raw_rhs);

        lhs_t result{};
        
        detail::bit_shift_right(result, lhs, rhs);

        return big_integer{result};
    }

    [[nodiscard]] constexpr auto to_big_integer(auto const & value)  {
        return big_integer{detail::to_storage(value)};
    }
}

template<std::size_t NumBits>
struct std::numeric_limits<safe::_big_integer::interface::big_integer<NumBits>> {
    using T = safe::_big_integer::interface::big_integer<NumBits>;

    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = false;
    static constexpr std::size_t digits = NumBits; // FIXME: the standard says this should be 'int'
    static constexpr int digits10 = std::numeric_limits<T>::digits * std::log10(2);
    static constexpr int max_digits10 = 0;
    static constexpr int radix = 2;
    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;
    static constexpr bool traps = true; // NOTE: it _might_ trap for divide by '0'
    static constexpr bool tinyness_before = false;

    static constexpr auto min() noexcept -> T {
        return T{T{1} << ((T::num_elems * 32) - 1)};
    }

    static constexpr auto lowest() noexcept -> T {
        return min();
    }

    static constexpr auto max() noexcept -> T {
        return ~min();
    }

    static constexpr auto epsilon() noexcept -> T {
        return T{0};
    }

   static constexpr auto round_error() noexcept -> T {
        return T{0};
    }

    static constexpr auto infinity() noexcept -> T {
        return T{0};
    }

    static constexpr auto quiet_NaN() noexcept -> T {
        return T{0};
    }

    static constexpr auto signaling_NaN() noexcept -> T {
        return T{0};
    }

    static constexpr auto denorm_min() noexcept -> T {
        return T{0};
    }
};