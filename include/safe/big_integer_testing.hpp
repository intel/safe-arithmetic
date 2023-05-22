#pragma once

#include <safe/big_integer.hpp>

#include <iostream>
#include <iomanip>
#include <algorithm>

namespace safe {
    template<std::size_t NumBits>
    void PrintTo(
            safe::big_integer<NumBits> const & v,
            std::ostream * os
    ) {
        for (auto i = std::rbegin(v.storage); i != std::rend(v.storage); i++) {
            *os << std::setfill('0') << std::hex << std::setw(8) << (*i) << " ";
        }
    }
}