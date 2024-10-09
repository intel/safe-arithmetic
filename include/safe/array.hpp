#pragma once

#include <safe/constant.hpp>
#include <safe/dsl.hpp>
#include <safe/var.hpp>

#include <array>
#include <compare>

namespace safe {
template <typename T, std::size_t Size> struct array {
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
    template <typename... Us>
    constexpr explicit array(Us... values) : storage({values...}) {}

    [[nodiscard]] constexpr auto
    operator[](var<size_type, ival<0, Size - 1>> pos) -> reference {
        return storage[pos.unsafe_value()];
    }

    [[nodiscard]] constexpr auto
    operator[](var<size_type, ival<0, Size - 1>> pos) const -> const_reference {
        return storage[pos.unsafe_value()];
    }

    [[nodiscard]] constexpr auto
    at(var<size_type, ival<0, Size - 1>> pos) -> reference {
        return storage[pos.unsafe_value()];
    }

    [[nodiscard]] constexpr auto
    at(var<size_type, ival<0, Size - 1>> pos) const -> const_reference {
        return storage[pos.unsafe_value()];
    }

    [[nodiscard]] constexpr auto front() -> reference {
        return storage.front();
    }

    [[nodiscard]] constexpr auto front() const -> const_reference {
        return storage.front();
    }

    [[nodiscard]] constexpr auto back() -> reference { return storage.back(); }

    [[nodiscard]] constexpr auto back() const -> const_reference {
        return storage.back();
    }

    // NOTE: intentionally omitting data()

    [[nodiscard]] constexpr auto begin() -> iterator { return storage.begin(); }

    [[nodiscard]] constexpr auto begin() const -> const_iterator {
        return storage.begin();
    }

    [[nodiscard]] constexpr auto cbegin() const -> const_iterator {
        return storage.cbegin();
    }

    [[nodiscard]] constexpr auto end() -> iterator { return storage.end(); }

    [[nodiscard]] constexpr auto end() const -> const_iterator {
        return storage.end();
    }

    [[nodiscard]] constexpr auto cend() const -> const_iterator {
        return storage.cend();
    }

    [[nodiscard]] constexpr auto rbegin() -> reverse_iterator {
        return storage.rbegin();
    }

    [[nodiscard]] constexpr auto rbegin() const -> const_reverse_iterator {
        return storage.rbegin();
    }

    [[nodiscard]] constexpr auto crbegin() const -> const_reverse_iterator {
        return storage.crbegin();
    }

    [[nodiscard]] constexpr auto rend() -> reverse_iterator {
        return storage.rend();
    }

    [[nodiscard]] constexpr auto rend() const -> const_reverse_iterator {
        return storage.rend();
    }

    [[nodiscard]] constexpr auto crend() const -> const_reverse_iterator {
        return storage.crend();
    }

    [[nodiscard]] constexpr auto empty() const -> bool {
        return storage.empty();
    }

    [[nodiscard]] constexpr auto size() const -> size_type {
        return storage.size();
    }

    [[nodiscard]] constexpr auto max_size() const -> size_type {
        return storage.max_size();
    }

    constexpr void fill(T const &value) { storage.fill(value); }

    constexpr void swap(array &other) noexcept { storage.swap(other.storage); }

    [[nodiscard]] friend constexpr auto operator==(array const &lhs,
                                                   array const &rhs) -> bool {
        return lhs.storage == rhs.storage;
    }

    [[nodiscard]] friend constexpr auto operator<=>(array const &lhs,
                                                    array const &rhs) {
        return lhs.storage <=> rhs.storage;
    }
};

template <class T, class... U> array(T, U...) -> array<T, 1 + sizeof...(U)>;
} // namespace safe

namespace std {
template <std::size_t I, class T, std::size_t N>
[[nodiscard]] constexpr auto get(safe::array<T, N> &a) noexcept -> T & {
    return a[safe::constant<std::size_t, I>];
}

template <std::size_t I, class T, std::size_t N>
[[nodiscard]] constexpr auto get(safe::array<T, N> &&a) noexcept -> T && {
    return std::move(a)[safe::constant<std::size_t, I>];
}

template <std::size_t I, class T, std::size_t N>
[[nodiscard]] constexpr auto
get(safe::array<T, N> const &a) noexcept -> T const & {
    return a[safe::constant<std::size_t, I>];
}

template <std::size_t I, class T, std::size_t N>
[[nodiscard]] constexpr auto
get(safe::array<T, N> const &&a) noexcept -> T const && {
    return a[safe::constant<std::size_t, I>];
}

template <class T, std::size_t N>
constexpr void swap(safe::array<T, N> &lhs, safe::array<T, N> &rhs) noexcept {
    lhs.swap(rhs);
}

template <class T, std::size_t N>
struct tuple_size<safe::array<T, N>> : std::integral_constant<std::size_t, N> {
};

template <std::size_t I, class T, std::size_t N>
struct tuple_element<I, safe::array<T, N>> {
    using type = T;
};
} // namespace std
