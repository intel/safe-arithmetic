#pragma once


#include <rapidcheck/gtest.h>

#include <safe/big_integer/detail/storage.hpp>


namespace safe::_big_integer::detail {
    template<std::size_t LhsNumBits, std::size_t RhsNumBits>
    void is_commutative(auto op, storage<LhsNumBits> a, storage<RhsNumBits> b) {
        using result_storage = storage<std::max(LhsNumBits, RhsNumBits)>;
        result_storage r0{}; op(r0, a, b);
        result_storage r1{}; op(r1, b, a);
        RC_ASSERT(r0 == r1);
    }

    template<typename ResultType>
    void is_associative(auto op, auto a, auto b, auto c) {
        ResultType r0{}; op(r0, a, b);
        ResultType r1{}; op(r1, r0, c);

        ResultType r2{}; op(r2, b, c);
        ResultType r3{}; op(r3, a, r2);
        RC_ASSERT(r3 == r1);
    }

    template<std::size_t NumBits>
    void is_idempotent(auto op, storage<NumBits> a) {
        storage<NumBits> result{}; op(result, a, a);
        RC_ASSERT(a == result);
    }

    template<std::size_t NumBits, auto id>
    void identity(auto op, storage<NumBits> a) {
        storage<NumBits> result{}; op(result, a, id);
        RC_ASSERT(a == result);
    }

    template<std::size_t NumBits, auto val>
    void domination(auto op, storage<NumBits> a) {
        storage<NumBits> result{}; op(result, a, val);
        RC_ASSERT(val == result);
    }

    template<std::size_t DstBits, std::size_t SrcBits>
    void assigned_value_equals_original(storage<SrcBits> a) {
        storage<DstBits> result{};
        result = a;
        RC_ASSERT(result == a);
    }

    template<std::size_t DstBits, std::size_t SrcBits>
    void copied_value_equals_original(storage<SrcBits> a) {
        storage<DstBits> result{a};
        RC_ASSERT(result == a);
    }

    template<typename ResultType>
    void lhs_can_be_result(auto op, auto a, auto b) {
        ResultType result0{}; op(result0, a, b);
        ResultType result1{a}; op(result1, result1, b);
        RC_ASSERT(result0 == result1);
    }
}