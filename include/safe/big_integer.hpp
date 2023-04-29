#pragma once

#include <safe/detail/fwd.hpp>

#include <cstddef>
#include <cstdint>
#include <array>
#include <concepts>
#include <type_traits>
#include <bit>


namespace safe {
    using big_integer_elem_t = std::uint32_t;
    using big_integer_double_elem_t = std::uint64_t;
    constexpr std::size_t big_integer_elem_num_bits = sizeof(big_integer_elem_t) * 8;

    template<std::size_t NumBits>
    struct big_integer {
    public:
        using elem_t = big_integer_elem_t;
        using double_elem_t = big_integer_double_elem_t;
        static constexpr std::size_t num_bits = NumBits;
        static constexpr std::size_t elem_num_bits = big_integer_elem_num_bits;
        static constexpr std::size_t num_elements = (num_bits + (elem_num_bits - 1)) / elem_num_bits;

        static_assert(num_bits > 0, "safe::big_integer num_bits must be greater than 0");

        // FIXME: make this private
        std::array<elem_t, num_elements> storage;

        SAFE_INLINE constexpr big_integer() = default;
        SAFE_INLINE constexpr big_integer(big_integer const &) = default;

        SAFE_INLINE constexpr big_integer(std::integral auto value)
            : storage{}
        {
            constexpr auto value_num_elems =
                std::min<std::size_t>(
                    num_elements,
                    (sizeof(value) + sizeof(elem_t) - 1) / sizeof(elem_t));

            for (auto i = std::size_t{}; i < value_num_elems; i++) {
                storage[i] = static_cast<elem_t>(value >> (i * elem_num_bits));
            }
        }

        template<std::size_t RhsNumBits>
        requires(NumBits >= RhsNumBits)
        SAFE_INLINE constexpr big_integer(
            big_integer<RhsNumBits> const & rhs
        )
            : storage{}
        {
            copy_from_rhs(rhs);
        }

        template<std::size_t RhsNumBits>
        requires(NumBits >= RhsNumBits)
        SAFE_INLINE constexpr big_integer & operator=(
            big_integer<RhsNumBits> const & rhs
        ) {
            copy_from_rhs(rhs);
            return *this;
        }

        template<std::size_t RhsNumBits>
        SAFE_INLINE constexpr big_integer(
            unsafe_cast_ferry<big_integer<RhsNumBits>> const & rhs
        )
            : storage{}
        {
            copy_from_rhs(rhs.value());
        }

        template<std::size_t RhsNumBits>
        SAFE_INLINE constexpr big_integer & operator=(
            unsafe_cast_ferry<big_integer<RhsNumBits>> const & rhs
        ) {
            copy_from_rhs(rhs.value());
            return *this;
        }

        template<std::size_t RhsNumBits, big_integer<RhsNumBits> rhs>
        SAFE_INLINE constexpr big_integer(
            std::integral_constant<big_integer<RhsNumBits>, rhs>
        )
                : storage{}
        {
            copy_from_rhs(rhs);
        }

    private:
        template<std::size_t RhsNumBits>
        SAFE_INLINE constexpr void copy_from_rhs(
            big_integer<RhsNumBits> const & rhs
        ) {
            using rhs_t = big_integer<RhsNumBits>;

            constexpr auto num_common_elems =
                std::min(num_elements, rhs_t::num_elements);

            auto const remaining =
                std::copy_n(rhs.storage.begin(), num_common_elems, storage.begin());

            if constexpr (rhs_t::num_elements < num_elements) {
                std::fill(remaining, storage.end(), 0u);
            }
        }
    };

    template<std::integral T>
    big_integer(T) -> big_integer<(sizeof(T) * 8) + (std::is_unsigned_v<T> ? 1 : 0)>;



    template<std::size_t NumBits>
    [[nodiscard]] constexpr big_integer<NumBits> const & get_big_integer(
        big_integer<NumBits> const & value
    ) {
        return value;
    }

    template<std::size_t NumBits, big_integer<NumBits> Value>
    [[nodiscard]] constexpr big_integer<NumBits> const & get_big_integer(
        std::integral_constant<big_integer<NumBits>, Value>
    ) {
        return Value;
    }

    template<std::size_t NumBits>
    [[nodiscard]] constexpr std::size_t as_size(
        big_integer<NumBits> value
    ) {
        return value.storage[0];
    }

    [[nodiscard]] constexpr std::size_t as_size(
        std::size_t value
    ) {
        return value;
    }




    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator==(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    )
        -> bool
    {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);

        constexpr auto min_num_elements =
            std::min(lhs_t::num_elements, rhs_t::num_elements);

        for (auto i = std::size_t{}; i < min_num_elements; i++) {
            if (lhs.storage[i] != rhs.storage[i]) {
                return false;
            }
        }

        if constexpr (lhs_t::num_elements > rhs_t::num_elements) {
            for (auto i = rhs.num_elements; i < lhs.num_elements; i++) {
                if (lhs.storage[i] != 0u) {
                    return false;
                }
            }
        } else if constexpr (lhs_t::num_elements < rhs_t::num_elements) {
            for (auto i = lhs.num_elements; i < rhs.num_elements; i++) {
                if (rhs.storage[i] != 0u) {
                    return false;
                }
            }
        }

        return true;
    }

    namespace detail {
        template<
            ConvertableToBigInteger Lhs,
            ConvertableToBigInteger Rhs>
        [[nodiscard]] SAFE_INLINE constexpr auto unsigned_compare(
            Lhs const & raw_lhs,
            Rhs const & raw_rhs
        )
            -> std::strong_ordering
        {
            using lhs_t = as_big_integer_t<Lhs>;
            lhs_t const & lhs = get_big_integer(raw_lhs);
            using rhs_t = as_big_integer_t<Rhs>;
            rhs_t const & rhs = get_big_integer(raw_rhs);

            constexpr auto min_num_elements =
                std::min(lhs_t::num_elements, rhs_t::num_elements);

            if constexpr (lhs_t::num_elements > rhs_t::num_elements) {
                for (auto i = lhs_t::num_elements - 1; i >= min_num_elements; i--) {
                    if (lhs.storage[i] != 0u) {
                        return lhs.storage[i] <=> 0u;
                    }
                }
            } else if constexpr (lhs_t::num_elements < rhs_t::num_elements) {
                for (auto i = rhs_t::num_elements - 1; i >= min_num_elements; i--) {
                    if (0u != rhs.storage[i]) {
                        return 0u <=> rhs.storage[i];
                    }
                }
            }

            for (auto i_plus_one = min_num_elements; i_plus_one != 0; i_plus_one--) {
                auto i = i_plus_one - 1;
                if (lhs.storage[i] != rhs.storage[i]) {
                    return lhs.storage[i] <=> rhs.storage[i];
                }
            }

            return std::strong_ordering::equal;
        }
    }

    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator<=>(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    )
        -> std::strong_ordering
    {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);

        bool const lhs_negative = lhs.storage.back() >> (big_integer_elem_num_bits - 1);
        bool const rhs_negative = rhs.storage.back() >> (big_integer_elem_num_bits - 1);

        if (lhs_negative && !rhs_negative) {
            return std::strong_ordering::less;

        } else if (!lhs_negative && rhs_negative) {
            return std::strong_ordering::greater;

        } else if (!lhs_negative && !rhs_negative) {
            return safe::detail::unsigned_compare(lhs, rhs);

        } else {
            return safe::detail::unsigned_compare(-rhs, -lhs);
        }
    }


    template<ConvertableToBigInteger Value>
    [[nodiscard]] SAFE_INLINE constexpr auto operator~(
        Value const & raw_value
    ) {
        using value_t = as_big_integer_t<Value>;
        value_t const & value = get_big_integer(raw_value);

        using result_t = big_integer<value_t::num_bits>;
        auto result = result_t{};
        for (auto i = std::size_t{}; i < result_t::num_elements; i++) {
            result.storage[i] = ~value.storage[i];
        }
        return result;
    }

    namespace detail {
        auto const add_with_carry =
            [](
                auto lhs_elem,
                auto rhs_elem,
                auto & carry
            )
                -> big_integer_double_elem_t
            {
                big_integer_double_elem_t const double_elem_result =
                    static_cast<big_integer_double_elem_t>(lhs_elem) +
                    static_cast<big_integer_double_elem_t>(rhs_elem) +
                    static_cast<big_integer_double_elem_t>(carry);

                carry =
                    static_cast<big_integer_elem_t>(double_elem_result >> big_integer_elem_num_bits);

                return static_cast<big_integer_elem_t>(double_elem_result);
            };
    }


    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator+(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);

        using result_t = big_integer<std::max(lhs_t::num_bits, rhs_t::num_bits) + 1>;

        constexpr auto min_num_elements =
            std::min(lhs_t::num_elements, rhs_t::num_elements);

        auto result = result_t{};
        auto carry = big_integer_elem_t{};

        for (auto i = std::size_t{}; i < min_num_elements; i++) {
            result.storage[i] = detail::add_with_carry(lhs.storage[i], rhs.storage[i], carry);
        }

        if constexpr (lhs_t::num_elements > rhs_t::num_elements) {
            for (auto i = min_num_elements; i < lhs_t::num_elements; i++) {
                result.storage[i] = detail::add_with_carry(lhs.storage[i], 0u, carry);
            }

        } else if constexpr (lhs_t::num_elements < rhs_t::num_elements) {
            for (auto i = min_num_elements; i < rhs_t::num_elements; i++) {
                result.storage[i] = detail::add_with_carry(0u, rhs.storage[i], carry);
            }
        }

        constexpr auto max_num_elements =
            std::max(lhs_t::num_elements, rhs_t::num_elements);

        if constexpr (result_t::num_elements > max_num_elements) {
            result.storage[result_t::num_elements - 1] = carry;
        }

        return result;
    }


    template<ConvertableToBigInteger Value>
    [[nodiscard]] SAFE_INLINE constexpr auto operator-(
        Value const & raw_value
    ) {
        using value_t = as_big_integer_t<Value>;
        value_t const & value = get_big_integer(raw_value);

        return ~value + big_integer<1>{1u};
    }


    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator-(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    ) {
        auto const & lhs = get_big_integer(raw_lhs);
        auto const & rhs = get_big_integer(raw_rhs);

        return lhs + -rhs;
    }

    namespace detail {
        template<typename T, std::size_t Size>
        constexpr void enumerate(std::array<T, Size> const & a, auto op) {
            [&]<std::size_t... i>(std::index_sequence<i...>){
                (op.template operator()<i>(a[i]), ...);
            }(std::make_index_sequence<Size>{});
        }

        template<typename T, std::size_t Size>
        constexpr auto accumulate(std::array<T, Size> const & a, auto init, auto op) {
            return [&]<std::size_t... i>(std::index_sequence<i...>){
                return (op.template operator()<i>(a[i]) + ... + init);
            }(std::make_index_sequence<Size>{});
        }

        template<
            ConvertableToBigInteger Lhs,
            ConvertableToBigInteger Rhs>
        [[nodiscard]] SAFE_INLINE constexpr auto unsigned_multiply(
            Lhs const & raw_lhs,
            Rhs const & raw_rhs
        ) {
            using lhs_t = as_big_integer_t<Lhs>;
            lhs_t const & lhs = get_big_integer(raw_lhs);
            using rhs_t = as_big_integer_t<Rhs>;
            rhs_t const & rhs = get_big_integer(raw_rhs);

            using result_t = big_integer<lhs_t::num_bits + rhs_t::num_bits>;
            auto carry = result_t{};
            auto result = result_t{};

            // NOTE: `enumerate` was chosen instead of `accumulate` due to compilation time
            detail::enumerate(lhs.storage, [&]<std::size_t i>(auto lhs_value){
                detail::enumerate(rhs.storage, [&]<std::size_t j>(auto rhs_value){
                    auto const partial_product =
                        static_cast<big_integer_double_elem_t>(lhs_value) *
                        static_cast<big_integer_double_elem_t>(rhs_value);

                    big_integer_double_elem_t const upper = partial_product >> 32;
                    big_integer_double_elem_t const lower = partial_product & 0xffff'ffffu;

                    constexpr auto elem_shift_amt = i + j;

                    big_integer_elem_t lower_carry{};
                    result.storage[elem_shift_amt] =
                        detail::add_with_carry(
                            result.storage[elem_shift_amt],
                            lower,
                            lower_carry);

                    if constexpr (elem_shift_amt + 1 < result_t::num_elements) {
                        carry.storage[elem_shift_amt + 1] += lower_carry;

                        big_integer_elem_t upper_carry{};
                        result.storage[elem_shift_amt + 1] =
                            detail::add_with_carry(
                                result.storage[elem_shift_amt + 1],
                                upper,
                                upper_carry);

                        if constexpr (elem_shift_amt + 2 < result_t::num_elements) {
                            carry.storage[elem_shift_amt + 2] += upper_carry;
                        }
                    }
                });
            });

            return unsafe_cast<result_t>(result + carry);
        }
    }


    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator*(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);

        using result_t = big_integer<lhs_t::num_bits + rhs_t::num_bits + 2>;

        constexpr auto zero = big_integer{0};

        if (lhs < zero) {
            if (rhs < zero) {
                return safe::detail::unsigned_multiply(-lhs, -rhs);
            } else {
                return -safe::detail::unsigned_multiply(-lhs, rhs);
            }
        } else {
            if (rhs < zero) {
                return -safe::detail::unsigned_multiply(lhs, -rhs);

            } else {
                return result_t{safe::detail::unsigned_multiply(lhs, rhs)};
            }
        }
    }

    template<
        ConvertableToBigInteger Lhs,
        typename T,
        T ShiftAmt>
    [[nodiscard]] SAFE_INLINE constexpr auto operator<<(
        Lhs const & raw_lhs,
        std::integral_constant<T, ShiftAmt> const &
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);

        constexpr std::size_t shift_amt = as_size(ShiftAmt);

        using result_t = big_integer<lhs_t::num_bits + shift_amt>;
        auto result = result_t{};

        constexpr std::size_t left_shift_amt = shift_amt % big_integer_elem_num_bits;
        constexpr std::size_t elem_shift_amt = shift_amt / big_integer_elem_num_bits;

        if constexpr (left_shift_amt == 0) {
            std::copy(begin(lhs.storage), end(lhs.storage), begin(result.storage) + elem_shift_amt);

        } else {
            constexpr std::size_t right_shift_amt = big_integer_elem_num_bits - left_shift_amt;
            for (auto i = std::size_t{}; i < lhs_t::num_elements; i++) {
                result.storage[i + elem_shift_amt + 1]  = lhs.storage[i] >> right_shift_amt;
                result.storage[i + elem_shift_amt    ] |= lhs.storage[i] << left_shift_amt;
            }
        }

        return result;
    }


    template<
        ConvertableToBigInteger Lhs,
        typename T,
        T ShiftAmt>
    [[nodiscard]] SAFE_INLINE constexpr auto operator>>(
        Lhs const & raw_lhs,
        std::integral_constant<T, ShiftAmt> const &
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);

        constexpr std::size_t shift_amt = as_size(ShiftAmt);

        if constexpr (shift_amt >= lhs_t::num_bits) {
            return big_integer<1>{0u};
        }

        using result_t = big_integer<lhs_t::num_bits - shift_amt>;
        auto result = result_t{};

        constexpr std::size_t right_shift_amt = shift_amt % big_integer_elem_num_bits;
        constexpr std::size_t elem_shift_amt = shift_amt / big_integer_elem_num_bits;

        if constexpr (right_shift_amt == 0) {
            std::copy(begin(lhs.storage) + elem_shift_amt, end(lhs.storage), begin(result.storage));

        } else {
            constexpr std::size_t left_shift_amt = big_integer_elem_num_bits - right_shift_amt;
            for (auto i = std::size_t{}; i < result_t::num_elements; i++) {
                auto const lhs_index = i + elem_shift_amt;
                result.storage[i] = lhs.storage[lhs_index] >> right_shift_amt;

                if ((lhs_index + 1) < lhs_t::num_elements) {
                    result.storage[i] |= (lhs.storage[lhs_index + 1] << left_shift_amt);
                }
            }
        }

        return result;
    }


    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator&(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);

        using result_t = big_integer<std::min(lhs_t::num_bits, rhs_t::num_bits)>;

        auto result = result_t{};
        for (auto i = std::size_t{}; i < result_t::num_elements; i++) {
            result.storage[i] = lhs.storage[i] & rhs.storage[i];
        }
        return result;
    }


    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator|(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);

        using result_t = big_integer<std::max(lhs_t::num_bits, rhs_t::num_bits)>;

        constexpr auto num_common_elems =
            std::min(lhs_t::num_elements, rhs_t::num_elements);

        auto result = result_t{};

        for (auto i = std::size_t{}; i < num_common_elems; i++) {
            result.storage[i] = lhs.storage[i] | rhs.storage[i];
        }

        if constexpr (lhs_t::num_elements > rhs_t::num_elements) {
            for (auto i = num_common_elems; i < lhs_t::num_elements; i++) {
                result.storage[i] = lhs.storage[i];
            }

        } else if constexpr (lhs_t::num_elements < rhs_t::num_elements) {
            for (auto i = num_common_elems; i < rhs_t::num_elements; i++) {
                result.storage[i] = rhs.storage[i];
            }
        }

        return result;
    }

    template<
        ConvertableToBigInteger Lhs,
        ConvertableToBigInteger Rhs>
    [[nodiscard]] SAFE_INLINE constexpr auto operator^(
        Lhs const & raw_lhs,
        Rhs const & raw_rhs
    ) {
        using lhs_t = as_big_integer_t<Lhs>;
        lhs_t const & lhs = get_big_integer(raw_lhs);
        using rhs_t = as_big_integer_t<Rhs>;
        rhs_t const & rhs = get_big_integer(raw_rhs);
        using result_t = big_integer<std::max(lhs_t::num_bits, rhs_t::num_bits)>;

        constexpr auto min_num_elements =
            std::min(lhs_t::num_elements, rhs_t::num_elements);

        auto result = result_t{};

        for (auto i = std::size_t{}; i < min_num_elements; i++) {
            result.storage[i] = lhs.storage[i] ^ rhs.storage[i];
        }

        if constexpr (lhs_t::num_elements > rhs_t::num_elements) {
            for (auto i = min_num_elements; i < lhs_t::num_elements; i++) {
                result.storage[i] = lhs.storage[i];
            }

        } else if constexpr (lhs_t::num_elements < rhs_t::num_elements) {
            for (auto i = min_num_elements; i < rhs_t::num_elements; i++) {
                result.storage[i] = rhs.storage[i];
            }
        }

        return result;
    }


    namespace literals {
        namespace detail {
            template<std::integral auto value>
            [[nodiscard]] consteval auto minimal_big_integer_impl() {
                constexpr auto type_digits =
                    std::numeric_limits<decltype(value)>::digits;

                constexpr auto leading_sign_digits = []() {
                    if constexpr (std::signed_integral<decltype(value)> && value < 0) {
                        return std::countl_one(value);

                    } else {
                        return std::countl_zero(value);
                    }
                }();

                constexpr auto sign_digit = 1;
                constexpr auto precision = type_digits - leading_sign_digits;

                return big_integer<sign_digit + precision>{value};
            }

            template<auto value>
            constexpr auto minimal_big_integer = minimal_big_integer_impl<value>();

            template<big_integer value>
            constexpr auto minimize_big_integer = [](){
                return value;
            }();

            template<char c>
            constexpr auto digit_value = [](){
                constexpr bool is_dec_digit = c >= '0' && c <= '9';
                constexpr bool is_upper_hex_digit = c >= 'A' && c <= 'F';
                constexpr bool is_lower_hex_digit = c >= 'a' && c <= 'f';

                static_assert(is_dec_digit || is_upper_hex_digit || is_lower_hex_digit);

                if constexpr (is_dec_digit) {
                    return minimal_big_integer<static_cast<std::uint32_t>(c - '0')>;

                } else if constexpr (is_upper_hex_digit) {
                    return minimal_big_integer<static_cast<std::uint32_t>(c - 'A' + 10)>;

                } else if constexpr (is_lower_hex_digit) {
                    return minimal_big_integer<static_cast<std::uint32_t>(c - 'a' + 10)>;
                }
            }();

            template<std::size_t base, big_integer upper, char lower, char... rest>
            [[nodiscard]] constexpr auto calc_literal_value() {
                constexpr auto base_bi =
                    detail::minimal_big_integer<base>;

                if constexpr (sizeof...(rest) == 0) {
                    return (upper * base_bi) + digit_value<lower>;

                } else if constexpr (lower == '\'') {
                    return calc_literal_value<base, upper, rest...>();

                } else {
                    return calc_literal_value<base, (upper * base_bi) + digit_value<lower>, rest...>();
                }
            }

            template<std::size_t base, big_integer upper, char, char, char... rest>
            [[nodiscard]] constexpr auto eat_two_then_calc_literal_value() {
                return calc_literal_value<base, upper, rest...>();
            }
        }

        template <char... chars>
        constexpr auto operator""_i() {
            constexpr auto zero =
                detail::minimal_big_integer<0u>;

            constexpr auto base =
                []() -> std::size_t {
                    constexpr auto s = std::array{chars...};

                    if (s.size() > 2) {
                        if (s[1] == 'x') {
                            return 16u;

                        } else if (s[1] == 'b') {
                            return 2u;

                        } else if (s[0] == '0') {
                            return 8u;

                        } else {
                            return 10u;
                        }

                    } else {
                        return 10u;
                    }
                }();

            constexpr auto result = [&] {
                if constexpr (base == 2u || base == 16u) {
                    return detail::eat_two_then_calc_literal_value<base, zero, chars...>();

                } else {
                    return detail::calc_literal_value<base, zero, chars...>();
                }
            }();

            constexpr auto min_result =
                detail::minimize_big_integer<result>;

            return std::integral_constant<std::remove_const_t<decltype(min_result)>, min_result>{};
        }
    }

    namespace detail {
        template<
            std::size_t LhsNumBits,
            std::size_t RhsNumBits>
        constexpr SAFE_INLINE auto largest_doubling(
            big_integer<LhsNumBits> a,
            big_integer<RhsNumBits> b
        ) {
            using namespace safe::literals;
            using ret_t = big_integer<LhsNumBits>;

            ret_t ret = unsafe_cast<ret_t>(b);

            while (ret <= (a - ret)) {
                ret = unsafe_cast<ret_t>(ret << 1_i);
            }

            return ret;
        }

        template<typename Q, typename R>
        struct divmod_result {
            using quotient_t = Q;
            using remainder_t = R;
            Q quotient;
            R remainder;
        };

        template<
            ConvertableToBigInteger Dividend,
            ConvertableToBigInteger Divisor>
        [[nodiscard]] constexpr SAFE_INLINE auto unsigned_divmod(
            Dividend const & raw_dividend,
            Divisor const & raw_divisor
        ) {
            using dividend_t = as_big_integer_t<Dividend>;
            dividend_t a = get_big_integer(raw_dividend);
            using divisor_t = as_big_integer_t<Divisor>;
            divisor_t const & b = get_big_integer(raw_divisor);

            using quotient_t = dividend_t;
            using remainder_t = dividend_t;

            using ret_t = detail::divmod_result<quotient_t, remainder_t>;

            // NOTE: Algorithm is based on "Elements of Programming"
            // section 5.7 Quotient

            if (a < b) {
                return ret_t{0u, a};

            } else {
                using namespace safe::literals;

                remainder_t c = safe::detail::largest_doubling(a, b);
                a = unsafe_cast<dividend_t>(a - c);
                quotient_t n = 1_i;
                while (c != b) {
                    n = unsafe_cast<quotient_t>(n << 1_i);
                    c = c >> 1_i;
                    if (c <= a) {
                        a = unsafe_cast<dividend_t>(a - c);
                        n = unsafe_cast<quotient_t>(n + 1_i);
                    }
                }

                return ret_t{n, a};
            }
        }
    }



    template<
        ConvertableToBigInteger Dividend,
        ConvertableToBigInteger Divisor>
    [[nodiscard]] constexpr SAFE_INLINE auto divmod(
        Dividend const & raw_dividend,
        Divisor const & raw_divisor
    ) {
        using namespace safe::literals;

        using dividend_t = as_big_integer_t<Dividend>;
        dividend_t a = get_big_integer(raw_dividend);
        using divisor_t = as_big_integer_t<Divisor>;
        divisor_t const & b = get_big_integer(raw_divisor);

        using quotient_t = dividend_t;
        using remainder_t = dividend_t;

        using ret_t = safe::detail::divmod_result<quotient_t, remainder_t>;

        constexpr auto unsafe_cast_ret = [](auto q_r) {
            return ret_t{
                unsafe_cast<typename ret_t::quotient_t>(q_r.quotient),
                unsafe_cast<typename ret_t::remainder_t>(q_r.remainder)
            };
        };

        // NOTE: Algorithm is based on "Elements of Programming"
        // section 5.8 Quotient and Remainder for Negative Quantities


        if (a < 0_i) {
            if (b < 0_i) {
                auto q_r = safe::detail::unsigned_divmod(-a, -b);
                q_r.remainder = unsafe_cast<remainder_t>(-q_r.remainder);
                return unsafe_cast_ret(q_r);
            } else {
                auto q_r = safe::detail::unsigned_divmod(-a, b);
                if (q_r.remainder != 0_i) {
                    q_r.remainder = unsafe_cast<remainder_t>(b - q_r.remainder);
                    q_r.quotient = unsafe_cast<quotient_t>(q_r.quotient + 1_i);
                }
                q_r.quotient = unsafe_cast<quotient_t>(-q_r.quotient);
                return unsafe_cast_ret(q_r);
            }
        } else {
            if (b < 0_i) {
                auto q_r = safe::detail::unsigned_divmod(a, -b);
                if (q_r.remainder != 0_i) {
                    q_r.remainder = unsafe_cast<remainder_t>(b + q_r.remainder);
                    q_r.quotient = unsafe_cast<quotient_t>(q_r.quotient + 1_i);
                }
                q_r.quotient = unsafe_cast<quotient_t>(-q_r.quotient);
                return unsafe_cast_ret(q_r);
            } else {
                auto q_r = safe::detail::unsigned_divmod(a, b);
                return unsafe_cast_ret(q_r);
            }
        }
    }

    template<
        ConvertableToBigInteger Dividend,
        ConvertableToBigInteger Divisor>
    [[nodiscard]] constexpr SAFE_INLINE auto operator/(
        Dividend const & lhs,
        Divisor const & rhs
    ) {
        return divmod(lhs, rhs).quotient;
    }

    template<
        ConvertableToBigInteger Dividend,
        ConvertableToBigInteger Divisor>
    [[nodiscard]] constexpr SAFE_INLINE auto operator%(
        Dividend const & lhs,
        Divisor const & rhs
    ) {
        return divmod(lhs, rhs).remainder;
    }
}