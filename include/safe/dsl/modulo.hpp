#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/union.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/big_integer.hpp>

#include <cstdint>


/*
 * C++ `%` operator is not modulo, but it is close.
 *
 *  a % -b  ==   a % b
 * -a %  b  == -(a % b)
 *
 * return [0, rhs.max - 1] (worst case)
 *                                   -0+                       |-- lhs --|
 *                                   -0+            |-- rhs --|
 *
 * return [0, lhs.max - lhs.min] U [lhs.min, lhs.max - 1]
 *                                   -0+            |-- lhs --|
 *                                   -0+            |-- rhs --|
 *
 * return lhs
 *                                   -0+ |-- lhs --|
 *                                   -0+            |-- rhs --|
 *
 * return lhs
 *                       |-- lhs --| -0+ :   lhs   :
 *            :   rhs   :            -0+            |-- rhs --|
 *
 * return [abs(lhs.max - lhs.min), 0] U [lhs.min + 1, lhs.max] // [lhs.min + 1, 0]
 *            |-- lhs --|            -0+            :   lhs   :
 *            :   rhs   :            -0+            |-- rhs --|
 *
 * return -[0, rhs.max - 1] (worst case)
 * |-- lhs --|                       -0+                       :   lhs   :
 *            :   rhs   :            -0+            |-- rhs --|
 *
 */

namespace safe::dsl {
    template<typename T, typename U>
    struct modulo : public detail::binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    requires (0 < rhs_min && rhs_max < lhs_min)
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {
        using type = ival_t<0, rhs_max - 1>;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    requires (
        lhs_min > 0 && 
        lhs_min == rhs_min && 
        lhs_max == rhs_max)
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {
        using type = detail::simplify_t<
            union_t<
                ival_t<0, lhs_max - lhs_min>, 
                ival_t<lhs_min, lhs_max - 1>
            >
        >;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    requires (
        0 <= lhs_min && 
        lhs_max < rhs_min)
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {
        using type = ival_t<lhs_min, lhs_max>;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    requires (
        lhs_max <= 0 && 
        0 < rhs_min &&
        -lhs_min < rhs_min)
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {
        using type = ival_t<lhs_min, lhs_max>;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    requires (
        0 < rhs_min &&
        -lhs_min == rhs_max &&
        -lhs_max == rhs_min)
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {        
        using type = detail::simplify_t<
            union_t<
                ival_t<-(lhs_max - lhs_min), 0>, 
                ival_t<lhs_min + 1, lhs_max>
            >
        >;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    requires (
        0 < rhs_min && 
        rhs_max < -lhs_max)
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {
        using type = ival_t<-(rhs_max - 1), 0>;
    };

    template<
        Operand LhsT,
        Operand RhsT>
    [[nodiscard]] constexpr auto operator%(LhsT, RhsT)
        -> modulo<LhsT, RhsT>
    {
        return {};
    }
}