#pragma once


namespace safe {
    template<typename T, auto Requirement>
    struct var;

    template <typename T>
    constexpr bool is_var_v = false;

    template<typename T, auto Requirement>
    constexpr bool is_var_v<var<T, Requirement>> = true;

    template<class T>
    concept Var = is_var_v<T>;

    [[nodiscard]] inline constexpr auto value(auto value);

    namespace detail {
        template<typename U, U value>
        [[nodiscard]] inline constexpr auto make_constant();
    }
}