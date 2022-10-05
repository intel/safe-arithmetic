#pragma once

#include <safe/contract.hpp>

namespace safe {
    template<typename T, std::size_t Size>
    struct array {
    private:
        std::array<T, Size> storage;

    public:
        constexpr T & operator[](
            contract<std::size_t, interval<0, Size - 1>> index
        ) {
            return storage[index.unsafe_value()];
        }
    };
}