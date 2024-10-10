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

template <char Char> constexpr bool is_0_v = Char == '0';
template <char Char> constexpr bool is_x_v = Char == 'x';

template <char Char>
constexpr bool is_hex_char_v =
    (Char >= 'A' && Char <= 'F' || Char >= 'a' && Char <= 'f');

template <char Char>
constexpr bool is_hex_digit_v = is_decimal_digit_v<Char> || is_hex_char_v<Char>;

template <char Char0, char Char1, char... Chars>
concept hex_number =
    is_0_v<Char0> && is_x_v<Char1> && (is_hex_digit_v<Chars> && ...);

template <typename T, char Char0, char Char1, char... Chars>
concept hex_integer =
    std::unsigned_integral<T> && hex_number<Char0, Char1, Chars...>;
} // namespace safe::detail
