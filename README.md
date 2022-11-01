# `safe::arithmetic`

[![Unit Tests](https://github.com/intel-innersource/libraries.cpp.functional.safe-vars/actions/workflows/unit_tests.yml/badge.svg)](https://github.com/intel-innersource/libraries.cpp.functional.safe-vars/actions/workflows/unit_tests.yml)

`safe::arithmetic` is a general purpose C++17 library for writing safe and 
bug-free code. It allows variables and functions to advertise and enforce
vars at compile-time. The vars are guaranteed to be held true at
runtime.

## Examples

```c++
using namespace safe::interval_types;
using namespace safe::literals;
```

`enqueue_index` is guaranteed to be 0 through 1023 inclusive. It is impossible
to assign a value to this variable outside this range.

```c++
ival_s32<0, 1023> enqueue_index = 0_s32;
```

We can perform arithmetic operations and prove we will not overflow. If we 
try to just add '1' to enqueue_index, we could overflow

```c++
enqueue_index = enqueue_index + 1_s32; // <- COMPILE ERROR 
```
Instead we are required to keep the value in-bounds, in this case we choose
to use the modulo operator.

```c++
enqueue_index = (enqueue_index + 1_s32) % 1024_s32; // GOOD!
```

for cases in which we must index into an array, the safe::arithmetic library
provides an array implementation with compile-time bounds checking.

```c++
safe::array<int, 1024> queue_data{};
queue_data[enqueue_index] = 0xc001;
```

It is not possible to index into the `safe::array` with a raw integral value or
a `safe::var` with an interval outside the bounds of the array.

```c++
queue_data[4]; // <- COMPILE ERROR
queue_data[enqueue_index + 1_s32]; // <- COMPILE ERROR
```

We must prove to the `safe::array` that the index value is within bounds. The
`safe::array` advertises this requirement as a `safe::var` as the index
parameter.

```c++
constexpr T & operator[](
    safe::var<std::size_t, safe::interval<0, Size - 1>> index
) {
    return storage[index.unsafe_value()];
}
```

User-code can (and should) do the same: use `safe::arithmetic` to specify the
requirements or assumptions about the input to the function. It is up to the
caller to prove the values it is passing in are safe.

More complex numerical requirements can be conveyed with `safe::arithmetic`. For
example, a disjoint union of intervals can be used in a var to exclude 
values or value ranges.

For example, if a '0' is invalid, but all other values are OK, a union of 
intervals can be used in the `safe::var`:

```c++
constexpr void dont_give_me_zero(
    safe::var<int, ival(-1000, 1) || ival(1, 1000)> not_zero
) {
    // ... do something really cool with this non-zero value ...
}
```

This makes it impossible to pass a value of '0' to this function. `safe::arithmetic`
can't be created or initialized with naked integral values, so how do we pass 
in parameters? We either need to use a `safe::var` that is already proven
to satisfy the callees vars, or we can use `safe::invoke`:

```c++
bool fail = safe::invoke(dont_give_me_zero, 0); // DOES NOT CALL FUNCTION
bool pass = safe::invoke(dont_give_me_zero, 42); // CALLS FUNCTION
```

`safe::invoke` will check the values at runtime if necessary to prove they
satisfy the `safe::arithmetic` of the function arguments. If any of them fail,
then the function is not called.