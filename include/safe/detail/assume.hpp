#pragma once

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#ifdef SAFE_TESTING
// if testing is turned on, turn assumptions into the appropriate framework's
// assertion
#ifdef RC_ASSERT
// RapidCheck
#define SAFE_ASSUME(expr) RC_ASSERT(expr)

#elif defined(REQUIRE)
// Catch2
#define SAFE_ASSUME(expr) REQUIRE(expr)

#elif defined(ASSERT_TRUE)
// GoogleTest
#define SAFE_ASSUME(expr) ASSERT_TRUE(expr)

#else
#define SAFE_ASSUME(expr)
#endif
#else
// adapted from section 4 in
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p1774r4.pdf
#ifdef __cpp_assume
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
#define SAFE_ASSUME(expr)                                                      \
    if (expr) {                                                                \
    } else {                                                                   \
        __builtin_unreachable();                                               \
    }
#endif
#elif defined(_MSC_VER) || defined(__ICC)
#define SAFE_ASSUME(expr) __assume(expr)
#endif
#endif

// NOLINTEND(cppcoreguidelines-macro-usage)
