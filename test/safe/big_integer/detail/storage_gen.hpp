#pragma once

#include <safe/big_integer/detail/storage.hpp>

#include <rapidcheck/gtest.h>

#include <iostream>

namespace safe::_big_integer::detail {
template <std::size_t NumBits>
std::ostream &operator<<(std::ostream &os, storage<NumBits> const &s) {
    auto i = s.num_elems;
    do {
        i--;
        os << std::setfill('0') << std::hex << std::setw(8) << s.get(i) << " ";
    } while (i > 0);
    return os;
}
} // namespace safe::_big_integer::detail

namespace rc {
template <std::size_t NumBits>
using safe_storage = safe::_big_integer::detail::storage<NumBits>;

template <std::size_t NumBits> struct Arbitrary<safe_storage<NumBits>> {
    using T = safe_storage<NumBits>;
    static Gen<T> arbitrary() {
        using src_array = std::array<uint32_t, T::num_elems>;

        return gen::map(gen::arbitrary<src_array>(),
                        [](src_array const &values) { return T{values}; });
    }
};
} // namespace rc