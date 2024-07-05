#pragma once

#include <concepts>

namespace safe::detail {
template <class I>
concept iter_like = requires(I i) {
    i++;
    *i;
    i != i;
};

template <class R>
concept range_like = requires(R r) {
    { r.begin() } -> iter_like;
    r.end();
};

template <char Char>
constexpr bool is_decimal_digit_v = Char >= '0' && Char <= '9';

template <char... Chars>
concept decimal_number = (is_decimal_digit_v<Chars> && ...);

template <typename T, char... Chars>
concept decimal_integer = std::integral<T> && decimal_number<Chars...>;
} // namespace safe::detail
