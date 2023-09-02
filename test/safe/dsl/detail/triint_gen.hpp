#pragma once

#include <safe/dsl/detail/triint.hpp>

#include <rapidcheck/gtest.h>

namespace safe::dsl::detail {
template <typename T>
std::ostream &operator<<(std::ostream &os, triint<T> const &t) {
    os << "triint(" << std::setfill('0') << std::hex << std::setw(sizeof(T) * 2)
       << t.var_bits_ << ", " << std::setfill('0') << std::hex
       << std::setw(sizeof(T) * 2) << t.const_bits_ << ")";

    return os;
}
} // namespace safe::dsl::detail

namespace rc {
template <typename T> struct Arbitrary<safe::dsl::triint<T>> {
    static Gen<safe::dsl::triint<T>> arbitrary() {
        return gen::apply(
            [](T var_bits, T const_bits) {
                return safe::dsl::triint{var_bits, const_bits};
            },
            gen::arbitrary<T>(), gen::arbitrary<T>());
    }
};
} // namespace rc