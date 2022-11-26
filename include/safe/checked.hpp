#pragma once

#include <limits>
#include <type_traits>

namespace safe {
    namespace detail {
        /*
         * Compute log2 of integral types.
         */
        template<typename T>
        [[nodiscard]] constexpr int log2(T value) {
            int result = 0;

            while (value != static_cast<T>(0)) {
                value = value / static_cast<T>(2);
                result++;
            }

            return result;
        }
    }

    template<typename T>
    struct checked {
    private:
        T const value_;
        bool const overflow_;

        template<typename U>
        friend struct checked;

    public:
        constexpr checked(T value)
            : value_{value}
            , overflow_{false}
        {}

        constexpr checked(T value, bool overflow)
            : value_{value}
            , overflow_{overflow}
        {}

        constexpr checked(checked const & rhs)
            : value_{rhs.value()}
            , overflow_{rhs.overflow_}
        {}

        [[nodiscard]] constexpr T value() const {
            return value_;
        }

        [[nodiscard]] constexpr bool is_overflow() const {
            return overflow_;
        }

        template<typename U>
        [[nodiscard]] friend constexpr checked<bool> operator==(checked<T> lhs, checked<U> rhs) {
            return {
                lhs.value() == rhs.value(),
                lhs.is_overflow() || rhs.is_overflow()
            };
        }

        template<typename U>
        [[nodiscard]] friend constexpr checked<bool> operator<(checked<T> lhs, checked<U> rhs) {
            if (lhs.is_overflow() | rhs.is_overflow()) {
                return {false, true};
            } else {
                return {lhs.value() < rhs.value(), false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr checked<bool> operator<=(checked<T> lhs, checked<U> rhs) {
            if (lhs.is_overflow() | rhs.is_overflow()) {
                return {false, true};
            } else {
                return {lhs.value() <= rhs.value(), false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr checked<bool> operator>(checked<T> lhs, checked<U> rhs) {
            if (lhs.is_overflow() | rhs.is_overflow()) {
                return {false, true};
            } else {
                return {lhs.value() > rhs.value(), false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr checked<bool> operator>=(checked<T> lhs, checked<U> rhs) {
            if (lhs.is_overflow() | rhs.is_overflow()) {
                return {false, true};
            } else {
                return {lhs.value() >= rhs.value(), false};
            }
        }

        // https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
        template<typename U>
        [[nodiscard]] friend constexpr auto operator+(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() + rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            constexpr auto zero = static_cast<ret_t>(0);
            constexpr auto max = std::numeric_limits<ret_t>::max();
            constexpr auto lowest = std::numeric_limits<ret_t>::lowest();

            bool const overflow =
                (right_value > zero) &&
                (left_value > max - right_value);

            bool const underflow = [&](){
                if constexpr (std::numeric_limits<ret_t>::is_signed) {
                    return
                        (right_value < zero) &&
                        (left_value < lowest - right_value);

                } else {
                    return false;
                }
            }();

            if (overflow | underflow | lhs.is_overflow() | rhs.is_overflow()) {
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{lhs.value() + rhs.value(), false};
            }
        }

        // https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
        template<typename U>
        [[nodiscard]] friend constexpr auto operator-(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() - rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            constexpr auto zero = static_cast<ret_t>(0);
            constexpr auto max = std::numeric_limits<ret_t>::max();
            constexpr auto lowest = std::numeric_limits<ret_t>::lowest();

            bool const overflow =
                (right_value < zero) &&
                (left_value > max + right_value);

            bool const underflow =
                (right_value > zero) &&
                (left_value < lowest + right_value);

            if (overflow | underflow | lhs.is_overflow() | rhs.is_overflow()) {
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{lhs.value() - rhs.value(), false};
            }
        }

        // https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
        // https://stackoverflow.com/questions/1815367/catch-and-compute-overflow-during-multiplication-of-two-large-integers
        template<typename U>
        [[nodiscard]] friend constexpr auto operator*(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() * rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            constexpr auto zero = static_cast<ret_t>(0);
            constexpr auto max = std::numeric_limits<ret_t>::max();
            constexpr auto lowest = std::numeric_limits<ret_t>::lowest();

            bool const overflow =
                (right_value > zero) &&
                (left_value > (max / right_value));

            bool const underflow = [&](){
                if constexpr (std::numeric_limits<ret_t>::is_signed) {
                    return
                        (right_value > zero) &&
                        (left_value < (lowest / right_value));

                } else {
                    return false;
                }
            }();

            bool const negation_overflow = [&](){
                if constexpr (
                    std::numeric_limits<ret_t>::is_signed &&
                    std::is_integral_v<ret_t>
                ) {
                    constexpr auto neg_one = static_cast<ret_t>(-1);

                    return
                        ((left_value == neg_one) && (right_value == lowest)) |
                        ((right_value == neg_one) && (left_value == lowest));

                } else {
                    return false;
                }
            }();

            if (overflow | underflow | negation_overflow | lhs.is_overflow() | rhs.is_overflow()) {
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{lhs.value() * rhs.value(), false};
            }
        }

        // https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
        template<typename U>
        [[nodiscard]] friend constexpr auto operator/(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() / rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            constexpr auto zero = static_cast<ret_t>(0);
            constexpr auto lowest = std::numeric_limits<ret_t>::lowest();

            bool const divide_by_zero =
                right_value == zero;

            bool const negation_overflow = [&](){
                if constexpr (
                    std::numeric_limits<ret_t>::is_signed &&
                    std::is_integral_v<ret_t>
                ) {
                    constexpr auto neg_one = static_cast<ret_t>(-1);

                    return
                        ((left_value == neg_one) && (right_value == lowest)) |
                        ((right_value == neg_one) && (left_value == lowest));

                } else {
                    return false;
                }
            }();

            if (divide_by_zero | negation_overflow | lhs.is_overflow() | rhs.is_overflow()) {
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{lhs.value() / rhs.value(), false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr auto operator%(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() % rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            constexpr auto zero = static_cast<ret_t>(0);

            bool const divide_by_zero =
                right_value == zero;

            if (divide_by_zero | lhs.is_overflow() | rhs.is_overflow()) {
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{left_value % right_value, false};
            }
        }


        template<typename U>
        [[nodiscard]] friend constexpr auto operator<<(checked<T> lhs, checked<U> rhs) {
            using ret_t = T;

            int const ret_bit_width = std::numeric_limits<ret_t>::digits;
            int const lhs_bit_width = detail::log2(lhs.value());
            int const shift_amt = rhs.value();

            bool const overflow =
                (lhs_bit_width + shift_amt) > ret_bit_width;

            bool const negative_shift =
                shift_amt < 0;

            if (overflow | negative_shift | lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{lhs.value() << shift_amt, false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr auto operator>>(checked<T> lhs, checked<U> rhs) {
            using ret_t = T;

            int const shift_amt = rhs.value();

            bool const negative_shift =
                shift_amt < 0;

            if (negative_shift | lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{lhs.value() >> shift_amt, false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr auto operator&(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() & rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            if (lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{left_value & right_value, false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr auto operator|(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() | rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            if (lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{left_value | right_value, false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr auto operator^(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() ^ rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            if (lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{left_value ^ right_value, false};
            }
        }


        template<typename U>
        [[nodiscard]] friend constexpr auto operator&&(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() && rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            if (lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{left_value && right_value, false};
            }
        }

        template<typename U>
        [[nodiscard]] friend constexpr auto operator||(checked<T> lhs, checked<U> rhs) {
            using ret_t =
                decltype(lhs.value() || rhs.value());

            auto const left_value = static_cast<ret_t>(lhs.value());
            auto const right_value = static_cast<ret_t>(rhs.value());

            if (lhs.is_overflow() | rhs.is_overflow()) {
                constexpr auto zero = static_cast<ret_t>(0);
                return checked<ret_t>{zero, true};
            } else {
                return checked<ret_t>{left_value || right_value, false};
            }
        }
    };

    template<auto V>
    struct checked_t {
        constexpr static auto value{V};
    };

    template<auto V>
    constexpr checked_t<V> checked_{};

    template<auto V>
    constexpr checked_t<V> c_{};

    template<auto lhs, auto rhs>
    [[nodiscard]] constexpr auto operator+(checked_t<lhs>, checked_t<rhs>) {
        constexpr auto result = checked{lhs} + checked{rhs};
        static_assert(!result.is_overflow());
        return result.value();
    }

    template<auto lhs, auto rhs>
    [[nodiscard]] constexpr auto operator-(checked_t<lhs>, checked_t<rhs>) {
        constexpr auto result = checked{lhs} - checked{rhs};
        static_assert(!result.is_overflow());
        return result.value();
    }

    template<auto lhs, auto rhs>
    [[nodiscard]] constexpr auto operator*(checked_t<lhs>, checked_t<rhs>) {
        constexpr auto result = checked{lhs} * checked{rhs};
        static_assert(!result.is_overflow());
        return result.value();
    }

    template<auto lhs, auto rhs>
    [[nodiscard]] constexpr auto operator/(checked_t<lhs>, checked_t<rhs>) {
        constexpr auto result = checked{lhs} / checked{rhs};
        static_assert(!result.is_overflow());
        return result.value();
    }

    template<auto lhs, auto rhs>
    [[nodiscard]] constexpr auto operator<<(checked_t<lhs>, checked_t<rhs>) {
        constexpr auto result = checked{lhs} << checked{rhs};
        static_assert(!result.is_overflow());
        return result.value();
    }

    template<auto lhs, auto rhs>
    [[nodiscard]] constexpr auto operator>>(checked_t<lhs>, checked_t<rhs>) {
        constexpr auto result = checked{lhs} >> checked{rhs};
        static_assert(!result.is_overflow());
        return result.value();
    }
}
