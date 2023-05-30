#pragma once


#include <cstdint>
#include <array>
#include <compare>


namespace safe::_big_integer::detail {
    using elem_t = std::uint32_t;
    using double_elem_t = std::uint64_t;

    template<std::size_t NumBits>
    struct storage {
    public:
        static constexpr auto num_elems = (NumBits + 31) / 32;

        constexpr storage() = default;

        constexpr storage(std::array<elem_t, num_elems> const & new_elems) {
            elems = new_elems;
        }

        template<std::size_t RhsNumBits>
        constexpr storage(storage<RhsNumBits> const & rhs) {
            for (auto i = std::size_t{}; i < NumBits; i++) {
                set(i, rhs.get(i));
            }
        };

        template<std::size_t RhsNumBits>
        constexpr auto operator=(storage<RhsNumBits> const & rhs) -> storage & {
            for (auto i = std::size_t{}; i < NumBits; i++) {
                set(i, rhs.get(i));
            }
            return *this;
        };

        template<std::size_t RhsNumBits>
        [[nodiscard]] constexpr auto operator==(
            storage<RhsNumBits> const & rhs
        ) const -> bool {
            for (auto i = std::size_t{}; i < std::max(num_elems, rhs.num_elems); i++) {
                if (get(i) != rhs.get(i)) {
                    return false;
                }
            }
            return true;
        }

        [[nodiscard]] constexpr bool negative() const {
            return (elems.back() >> 31) & 1;
        }

        [[nodiscard]] constexpr auto get(int32_t i) const -> elem_t {
            if (i < 0) {
                return 0u;
            } else if (i < num_elems) {
                return elems[i];
            } else {
                if (negative()) {
                    return 0xffff'ffffu;
                } else {
                    return 0u;
                }
            }
        }

        constexpr auto set(int32_t i, elem_t elem) -> void {
            if (i >= 0 && i < num_elems) {
                elems[i] = elem;
            }
        }

        std::array<elem_t, num_elems> elems{};
    };

    [[nodiscard]] constexpr auto make_storage(int32_t v) -> storage<32> {
        return storage<32>{{static_cast<uint32_t>(v)}};
    }

    [[nodiscard]] constexpr auto make_storage(int64_t v) -> storage<64> {
        return storage<64>{{
            static_cast<uint32_t>(v),
            static_cast<uint32_t>(v >> 32)
        }};
    }
}