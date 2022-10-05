#pragma once

#include <type_traits>

namespace safe {
    template<typename LhsT, typename RhsT>
    struct req_is_assignable : public std::integral_constant<bool, true> {};

    template<typename LhsT, typename RhsT>
    struct req_operator_plus {};

    template<typename LhsT, typename RhsT>
    struct req_operator_minus {};

    template<typename LhsT, typename RhsT>
    struct req_operator_multiply {};

    template<typename LhsT, typename RhsT>
    struct req_operator_divide {};

    template<typename LhsT, typename RhsT>
    struct req_operator_modulo {};

    template<typename LhsT, typename RhsT>
    struct req_operator_shift_left {};

    template<typename LhsT, typename RhsT>
    struct req_operator_shift_right {};

    template<typename T, typename... Requirements>
    struct contract;


    template<typename... Intervals>
    struct interval_union;
}