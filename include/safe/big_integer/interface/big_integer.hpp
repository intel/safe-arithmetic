#pragma once


#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/operators.hpp>
#include <safe/big_integer/interface/fwd.hpp>

#include <cstdint>


namespace safe::_big_integer::interface {
    template<std::size_t NumBits>
    struct big_integer {
    public:
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

        detail::storage<NumBits> unsafe_storage;

        constexpr auto do_assign_op(auto op, auto const & rhs) -> big_integer & {
            op(unsafe_storage, unsafe_storage, rhs.storage);
            return *this;
        }
    };

    [[nodiscard]] constexpr auto do_binary_op(
        auto op,
        auto width_calc,
        auto const & raw_lhs,
        auto const & raw_rhs
    ) {
        auto lhs = to_storage(raw_lhs);
        auto rhs = to_storage(raw_rhs);
        using lhs_t = std::remove_cvref_t<decltype(lhs)>;
        using rhs_t = std::remove_cvref_t<decltype(rhs)>;

        constexpr auto result_width = width_calc<lhs_t, rhs_t>();
        storage<result_width> result{};

        op(result, lhs, rhs);

        return big_integer<result_width>(result);
    }

//    [[nodiscard]] constexpr auto operator+(
//        auto const & raw_lhs,
//        auto const & raw_rhs
//    ) {
//        return do_binary_op(detail::plus, 0, raw_lhs, raw_rhs);
//    }
}