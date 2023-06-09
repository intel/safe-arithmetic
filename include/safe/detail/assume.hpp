#pragma once

// adapted from section 4 in https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p1774r4.pdf

#if defined(__cpp_assume)
    #define SAFE_ASSUME(expr) [[assume(expr)]]
#elif defined(__clang__)
    // https://clang.llvm.org/docs/LanguageExtensions.html#builtin-assume
    #define SAFE_ASSUME(expr) __builtin_assume(expr)
#elif defined(__GNUC__) && !defined(__ICC)
    #if __GNUC__ >= 13
        // https://gcc.gnu.org/onlinedocs/gcc-13.1.0/gcc/Statement-Attributes.html#index-assume-statement-attribute
        #define SAFE_ASSUME(expr) __attribute__((assume(expr)))
    #else
        // this is very questionable whether it does anything at all
        #define SAFE_ASSUME(expr) if (expr) {} else { __builtin_unreachable(); }
    #endif
#elif defined(_MSC_VER) || defined(__ICC)
    #define SAFE_ASSUME(expr) __assume(expr)
#endif