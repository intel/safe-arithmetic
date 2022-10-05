#pragma once

#include <safe/contract.hpp>
#include <safe/interval.hpp>
#include <safe/constant.hpp>

#include <cstdint>
#include <limits>

namespace safe {
    template<
        uint32_t min = std::numeric_limits<uint32_t>::min(),
        uint32_t max = std::numeric_limits<uint32_t>::max()>
    using uint32 = safe::contract<uint32_t, safe::interval<min, max>>;

    template<uint32_t value>
    constexpr auto uint32_ = constant<uint32_t, value>;

    template<
        int32_t min = std::numeric_limits<int32_t>::min(),
        int32_t max = std::numeric_limits<int32_t>::max()>
    using int32 = safe::contract<int32_t, safe::interval<min, max>>;

    template<int32_t value>
    constexpr auto int32_ = constant<int32_t, value>;

    template<
        uint64_t min = std::numeric_limits<uint64_t>::min(),
        uint64_t max = std::numeric_limits<uint64_t>::max()>
    using uint64 = safe::contract<uint64_t, safe::interval<min, max>>;

    template<uint64_t value>
    constexpr auto uint64_ = constant<uint64_t, value>;

    template<
        int64_t min = std::numeric_limits<int64_t>::min(),
        int64_t max = std::numeric_limits<int64_t>::max()>
    using int64 = safe::contract<int64_t, safe::interval<min, max>>;

    template<int64_t value>
    constexpr auto int64_ = constant<int64_t, value>;
}