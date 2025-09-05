#pragma once

#include <safe/big_integer/interface/fwd.hpp>

#include <array>
#include <compare>
#include <cstdint>
#include <type_traits>

namespace safe::_big_integer::detail {
using elem_t = std::uint32_t;
using double_elem_t = std::uint64_t;

template <std::size_t NumBits> struct storage {
  public:
    constexpr static std::size_t num_elems = (NumBits + 31) / 32;
    constexpr static std::size_t num_bits = NumBits;

    std::array<elem_t, num_elems> elems{};

    constexpr storage() = default;
    constexpr explicit storage(std::array<elem_t, num_elems> const &new_elems)
        : elems{new_elems} {}

    template <std::size_t RhsNumBits>
    constexpr explicit storage(storage<RhsNumBits> const &rhs) {
        for (auto i = std::size_t{}; i < NumBits; i++) {
            set(i, rhs.get(i));
        }
    };

    template <std::size_t RhsNumBits>
    constexpr auto operator=(storage<RhsNumBits> const &rhs) -> storage & {
        for (auto i = std::size_t{}; i < NumBits; i++) {
            set(i, rhs.get(i));
        }
        return *this;
    };

    template <std::size_t RhsNumBits>
    [[nodiscard]] constexpr auto
    operator==(storage<RhsNumBits> const &rhs) const -> bool {
        for (auto i = std::size_t{}; i < std::max(num_elems, rhs.num_elems);
             i++) {
            if (get(i) != rhs.get(i)) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] constexpr auto negative() const -> bool {
        return (elems.back() >> 31) & 1;
    }

    [[nodiscard]] constexpr auto get(int32_t i) const -> elem_t {
        if (i < 0) {
            return elem_t{};
        }
        if (auto iu = static_cast<std::size_t>(i); iu < num_elems) {
            return elems[iu];
        }
        if (negative()) {
            return 0xffff'ffffu;
        }
        return elem_t{};
    }

    constexpr auto set(int32_t i, elem_t elem) -> void {
        if (i >= 0) {
            if (auto iu = static_cast<std::size_t>(i); iu < num_elems) {
                elems[iu] = elem;
            }
        }
    }
};

template <typename T>
    requires(sizeof(T) <= 4 && std::signed_integral<std::remove_cvref_t<T>>)
[[nodiscard]] constexpr auto to_storage(T v) -> storage<sizeof(T) * 8> {
    return storage<sizeof(T) * 8>{
        {static_cast<uint32_t>(static_cast<int32_t>(v))}};
}

template <typename T>
    requires(sizeof(T) <= 4 && std::unsigned_integral<std::remove_cvref_t<T>>)
[[nodiscard]] constexpr auto to_storage(T v) -> storage<(sizeof(T) * 8) + 1> {
    return storage<(sizeof(T) * 8) + 1>{{static_cast<uint32_t>(v)}};
}

template <typename T>
    requires(sizeof(T) == 8 && std::signed_integral<std::remove_cvref_t<T>>)
[[nodiscard]] constexpr auto to_storage(T v) -> storage<64> {
    return storage<64>{
        {static_cast<uint32_t>(v), static_cast<uint32_t>(v >> 32)}};
}

template <typename T>
    requires(sizeof(T) == 8 && std::unsigned_integral<std::remove_cvref_t<T>>)
[[nodiscard]] constexpr auto to_storage(T v) -> storage<65> {
    return storage<65>{
        {static_cast<uint32_t>(v), static_cast<uint32_t>(v >> 32)}};
}

template <std::size_t NumBits>
[[nodiscard]] constexpr auto
to_storage(interface::big_integer<NumBits> const &v) -> auto const & {
    return v.unsafe_storage;
}

template <std::size_t NumBits>
[[nodiscard]] constexpr auto to_storage(storage<NumBits> &v) -> auto & {
    return v;
}

template <std::size_t NumBits>
[[nodiscard]] constexpr auto to_storage(storage<NumBits> const &v)
    -> auto const & {
    return v;
}

template <typename T, T value>
[[nodiscard]] constexpr auto to_storage(std::integral_constant<T, value>) {
    return to_storage(value);
}

[[nodiscard]] constexpr auto make_storage(auto v) { return to_storage(v); }

[[nodiscard]] constexpr auto to_integral(std::integral auto value) {
    return value;
}

template <std::integral T, T value>
[[nodiscard]] constexpr auto to_integral(std::integral_constant<T, value>) {
    return value;
}

template <std::size_t NumBits>
    requires(NumBits > 32 && NumBits <= 64)
[[nodiscard]] constexpr auto to_integral(storage<NumBits> const &value)
    -> int64_t {
    return (static_cast<uint64_t>(value.get(1)) << 32u) |
           (static_cast<uint64_t>(value.get(0)));
}

template <std::size_t NumBits>
    requires(NumBits <= 32)
[[nodiscard]] constexpr auto to_integral(storage<NumBits> const &value)
    -> int32_t {
    return static_cast<int32_t>(value.get(0));
}

template <std::size_t NumBits>
[[nodiscard]] constexpr auto
to_integral(interface::big_integer<NumBits> const &value) {
    return to_integral(value.unsafe_storage);
}

constexpr static auto max_width_plus_one =
    [](std::size_t left_bits, std::size_t right_bits) -> std::size_t {
    return std::max(left_bits, right_bits) + 1;
};

constexpr static auto max_width = [](std::size_t left_bits,
                                     std::size_t right_bits) -> std::size_t {
    return std::max(left_bits, right_bits);
};

constexpr static auto sum_width = [](std::size_t left_bits,
                                     std::size_t right_bits) -> std::size_t {
    return left_bits + right_bits;
};
} // namespace safe::_big_integer::detail
