#pragma once

#include <safe/var.hpp>
#include <safe/dsl.hpp>
#include <safe/constant.hpp>

#include <compare>
#include <array>

namespace safe {
    template<typename T, std::size_t Size>
    struct array {
    private:
        using std_array = std::array<T, Size>;
        std_array storage;

    public:
        using value_type = typename std_array::value_type;
        using size_type = typename std_array::size_type;
        using difference_type = typename std_array::difference_type;
        using reference = typename std_array::reference;
        using const_reference = typename std_array::const_reference;
        using pointer = typename std_array::pointer;
        using const_pointer = typename std_array::const_pointer;
        using iterator = typename std_array::iterator;
        using const_iterator = typename std_array::const_iterator;
        using reverse_iterator = typename std_array::reverse_iterator;
        using const_reverse_iterator = typename std_array::const_reverse_iterator;

        // TODO: constructors
        template<typename... Us>
        constexpr array(Us... values)
            : storage({values...})
        {}

        [[nodiscard]] constexpr reference operator[](
            var<size_type, ival<0, Size - 1>> pos
        ) {
            return storage[pos.unsafe_value()];
        }

        [[nodiscard]] constexpr const_reference operator[](
            var<size_type, ival<0, Size - 1>> pos
        ) const {
            return storage[pos.unsafe_value()];
        }

        [[nodiscard]] constexpr reference at(
            var<size_type, ival<0, Size - 1>> pos
        ) {
            return storage[pos.unsafe_value()];
        }

        [[nodiscard]] constexpr const_reference at(
            var<size_type, ival<0, Size - 1>> pos
        ) const {
            return storage[pos.unsafe_value()];
        }

        [[nodiscard]] constexpr reference front() {
            return storage.front();
        }

        [[nodiscard]] constexpr const_reference front() const {
            return storage.front();
        }

        [[nodiscard]] constexpr reference back() {
            return storage.back();
        }

        [[nodiscard]] constexpr const_reference back() const {
            return storage.back();
        }

        // NOTE: intentionally omitting data()

        [[nodiscard]] constexpr iterator begin() {
            return storage.begin();
        }

        [[nodiscard]] constexpr const_iterator begin() const {
            return storage.begin();
        }

        [[nodiscard]] constexpr const_iterator cbegin() const {
            return storage.cbegin();
        }

        [[nodiscard]] constexpr iterator end() {
            return storage.end();
        }

        [[nodiscard]] constexpr const_iterator end() const {
            return storage.end();
        }

        [[nodiscard]] constexpr const_iterator cend() const {
            return storage.cend();
        }

        [[nodiscard]] constexpr reverse_iterator rbegin() {
            return storage.rbegin();
        }

        [[nodiscard]] constexpr const_reverse_iterator rbegin() const {
            return storage.rbegin();
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const {
            return storage.crbegin();
        }

        [[nodiscard]] constexpr reverse_iterator rend() {
            return storage.rend();
        }

        [[nodiscard]] constexpr const_reverse_iterator rend() const {
            return storage.rend();
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const {
            return storage.crend();
        }

        [[nodiscard]] constexpr bool empty() const {
            return storage.empty();
        }

        [[nodiscard]] constexpr size_type size() const {
            return storage.size();
        }

        [[nodiscard]] constexpr size_type max_size() const {
            return storage.max_size();
        }

        constexpr void fill(T const & value) {
            storage.fill(value);
        }

        constexpr void swap(array & other) {
            storage.swap(other.storage);
        }

        [[nodiscard]] friend constexpr bool operator==(
            array const & lhs,
            array const & rhs
        ) {
            return lhs.storage == rhs.storage;
        }

        [[nodiscard]] friend constexpr auto operator<=>(
            array const & lhs,
            array const & rhs
        ) {
            return lhs.storage <=> rhs.storage;
        }
    };

    template <class T, class... U>
    array(T, U...) -> array<T, 1 + sizeof...(U)>;
}

namespace std {
    template<std::size_t I, class T, std::size_t N>
    [[nodiscard]] constexpr T & get(safe::array<T, N> & a) noexcept {
        return a[safe::constant<std::size_t, I>];
    }


    template<std::size_t I, class T, std::size_t N>
    [[nodiscard]] constexpr T && get(safe::array<T, N> && a) noexcept {
        return a[safe::constant<std::size_t, I>];
    }


    template<std::size_t I, class T, std::size_t N>
    [[nodiscard]] constexpr const T & get(safe::array<T, N> const & a) noexcept {
        return a[safe::constant<std::size_t, I>];
    }


    template<std::size_t I, class T, std::size_t N>
    [[nodiscard]] constexpr const T && get(safe::array<T, N> const && a) noexcept {
        return a[safe::constant<std::size_t, I>];
    }

    template<class T, std::size_t N>
    constexpr void swap(
        safe::array<T, N> & lhs,
        safe::array<T, N> & rhs
    ) {
        lhs.swap(rhs);
    }

    template<class T, std::size_t N>
    struct tuple_size<safe::array<T, N>>
        : std::integral_constant<std::size_t, N>
    {};

    template<std::size_t I, class T, std::size_t N>
    struct tuple_element<I, safe::array<T, N>> {
        using type = T;
    };
}