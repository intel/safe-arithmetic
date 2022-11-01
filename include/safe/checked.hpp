#pragma once

#include <limits>
#include <type_traits>

namespace safe {
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

        [[nodiscard]] constexpr T value() const {
            return value_;
        }

        [[nodiscard]] constexpr bool is_overflow() const {
            return overflow_;
        }

        template<typename U>
        [[nodiscard]] constexpr checked<bool> operator==(checked<U> rhs) const {
            return {value_ == rhs.value_ && std::is_same_v<T, U>, overflow_ || rhs.overflow_};
        }

        template<typename U>
        [[nodiscard]] constexpr auto operator+(checked<U> rhs) const {
            using ret_t =
                decltype(value_ + rhs.value_);

            bool const overflow =
                rhs.value_ > 0 && value_ > std::numeric_limits<ret_t>::max() - rhs.value_;

            bool const underflow = [&](){
                if constexpr (std::numeric_limits<ret_t>::is_signed) {
                    return rhs.value_ < 0 && value_ < std::numeric_limits<ret_t>::lowest() - rhs.value_;

                } else {
                    return false;
                }
            }();

            if (overflow || underflow || overflow_ || rhs.overflow_) {
                return checked<ret_t>{0, true};
            } else {
                return checked<ret_t>{value_ + rhs.value_, false};
            }
        }
    };
}
