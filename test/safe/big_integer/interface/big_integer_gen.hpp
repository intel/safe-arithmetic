#pragma once

#include <rapidcheck/gtest.h>

#include <safe/big_integer/detail/storage_gen.hpp>

#include <safe/big_integer/interface/big_integer.hpp>

namespace safe::_big_integer::interface {
    template<std::size_t NumBits>
    std::ostream & operator<<(std::ostream & os, big_integer<NumBits> const & b) {
        auto const & s = b.unsafe_storage;
        auto i = s.num_elems;
        do {
            i--;
            os << std::setfill('0') << std::hex << std::setw(8) << s.get(i) << " ";
        } while (i > 0);
        return os;
    }
}


namespace rc {
    template<std::size_t NumBits>
    using safe_big_integer = safe::_big_integer::interface::big_integer<NumBits>;

    template<std::size_t NumBits>
    struct Arbitrary<safe_big_integer<NumBits>> {
        using T = safe_big_integer<NumBits>;
        static Gen<T> arbitrary() {
            using src =
                safe::_big_integer::detail::storage<NumBits>;

            return gen::map(
                gen::arbitrary<src>(),
                [](src const & v) {
                    return T{v};
                }
            );
        }
    };
}