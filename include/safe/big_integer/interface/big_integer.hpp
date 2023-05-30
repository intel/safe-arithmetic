#pragma once


#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/operators.hpp>
#include <safe/big_integer/interface/fwd.hpp>

#include <cstdint>
#include <functional>


namespace safe::_big_integer::interface {
    template<std::size_t NumBits>
    struct big_integer {
        detail::storage<NumBits> unsafe_storage;

        constexpr big_integer(auto value) : unsafe_storage{detail::to_storage(value)} {}

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

    private:
        constexpr auto do_assign_op(auto op, auto const & rhs) -> big_integer & {
            op(unsafe_storage, unsafe_storage, rhs.storage);
            return *this;
        }
    };

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

   [[nodiscard]] constexpr auto operator+(
       auto const & raw_lhs,
       auto const & raw_rhs
   ) {
       return do_binary_op(detail::plus, detail::max_width_plus_one, raw_lhs, raw_rhs);
   }

   [[nodiscard]] constexpr auto operator-(
       auto const & raw_lhs,
       auto const & raw_rhs
   ) {
       return do_binary_op(detail::minus, detail::max_width_plus_one, raw_lhs, raw_rhs);
   }

   [[nodiscard]] constexpr auto operator*(
       auto const & raw_lhs,
       auto const & raw_rhs
   ) {
       return do_binary_op(detail::multiplies, detail::sum_width, raw_lhs, raw_rhs);
   }    
   
   [[nodiscard]] constexpr auto divmod(
        auto const & raw_lhs,
        auto const & raw_rhs
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

   [[nodiscard]] constexpr auto operator/(
       auto const & lhs,
       auto const & rhs
   ) {
       auto [q, r] = divmod(lhs, rhs);
       return q;
   }

   [[nodiscard]] constexpr auto operator%(
       auto const & lhs,
       auto const & rhs
   ) {
       auto [q, r] = divmod(lhs, rhs);
       return r;
   }

   [[nodiscard]] constexpr auto operator|(
       auto const & raw_lhs,
       auto const & raw_rhs
   ) {
       return do_binary_op(detail::bit_or, detail::max_width, raw_lhs, raw_rhs);
   }

   [[nodiscard]] constexpr auto operator&(
       auto const & raw_lhs,
       auto const & raw_rhs
   ) {
       return do_binary_op(detail::bit_and, detail::max_width, raw_lhs, raw_rhs);
   }

   [[nodiscard]] constexpr auto operator^(
       auto const & raw_lhs,
       auto const & raw_rhs
   ) {
       return do_binary_op(detail::bit_xor, detail::max_width, raw_lhs, raw_rhs);
   }
}