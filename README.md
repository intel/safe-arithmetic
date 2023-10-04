# safe arithmetic

*safe arithmetic* is a general purpose C++20 library for writing safe and 
bug-free code. It allows variables and functions to advertise and enforce
requirements at compile-time. The requirements are guaranteed to be held 
true at runtime.

[![CppCon 2023. Safe Arithmetic: Have your integer and add it to two!](https://img.youtube.com/vi/E1LfLQ13bgA/0.jpg)](https://www.youtube.com/watch?v=E1LfLQ13bgA)

## Pre-release Disclaimer

This library is a work in progress and should not yet be used in production.
It is being developed "in the open" at a very early stage not usually seen.
The API will change, there are definitely bugs, it may not even compile, and it
is not yet complete. It is currently undergoing large changes in the API, 
design, and implementation. If you are interested, please take a look at the
documentation on the webpage and provide feedback about the API, design, and
user guide.

## Motivation

There are a number of ways that seemingly innocent arithmetic operations can
result in functional bugs and/or security vulnerabilities. 

Here are some rules from the 
[SEI CERT C Coding Standard (2016)](https://resources.sei.cmu.edu/downloads/secure-coding/assets/sei-cert-c-coding-standard-2016-v01.pdf)
designed to prevent such bugs:
- EXP33-C. Do not read uninitialized memory
- INT30-C. Ensure that unsigned integer operations do not wrap
- INT31-C. Ensure that integer conversions do not result in lost or misinterpreted data
- INT32-C. Ensure that operations on signed integers do not result in overflow
- INT33-C. Ensure that division and remainder operations do not result in divide-by-zero errors
- INT34-C. Do not shift an expression by a negative number of bits or by greater than or equal to the number of bits that exist in the operand
- INT35-C. Use correct integer precisions
- ARR30-C. Do not form or use out-of-bounds pointers or array subscripts

Besides limitations of the language and discrete and finite representations of
numerical values, there are semantic and API requirements in libraries and
applications that will cause bugs if violated.

## Examples

Import a couple namespaces for convenient use.

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
Instead, we are required to keep the value in-bounds, in this case we choose
to use the modulo operator.

```c++
enqueue_index = (enqueue_index + 1_s32) % 1024_s32; // GOOD!
```

for cases in which we must index into an array, the *safe arithmetic* library
provides an array implementation with compile-time bounds checking.

```c++
safe::array<int, 1024> queue_data{};
queue_data[enqueue_index] = 0xc001;
```

It is not possible to index into the `safe::array` with a raw integral value or
a `safe::var` with an interval outside the bounds of the array.

```c++
auto result_err = queue_data[4]; // <- COMPILE ERROR
```

The `_u32` user-defined literal creates a `safe::var` type at compile time that
is constrained to the single value given to it. 

```c++
auto result = queue_data[4_u32]; // GOOD!
```

Arithmetic operations generate a new requirement for the result-type. Adding `1`
to `enqueue_index` means it could be one larger than the last element of the
array. The *safe arithmetic* library correctly produces a compilation error.

```c++
auto result = queue_data[enqueue_index + 1_u32]; // <- COMPILE ERROR
```

We must prove to the `safe::array` that the index value is within bounds. 
There are many ways to do this. For this queue implementation we want 
`enqueue_index` to wrap around.

```c++
auto result = queue_data[(enqueue_index + 1_u32) % 1024_u32]; // GOOD!
```

The `safe::array` advertises this requirement on the `safe::var` index
parameter.

```c++
constexpr T & operator[](
    safe::var<std::size_t, safe::ival<0, Size - 1>> index
) {
    return storage[index.unsafe_value()];
}
```

User-code can (and should) do the same: use `safe::var` to specify the
requirements or assumptions about the input to the function. It is up to the
caller to prove the values it is passing in are safe.

More complex numerical requirements can be conveyed with *safe arithmetic*. For
example, a disjoint union of intervals can be used in a requirement to exclude 
values or value ranges.

For example, if a `0` is invalid, but all other values are OK, a union of 
intervals can be used in the `safe::var`:

```c++
constexpr void dont_give_me_zero(
    safe::var<int, safe::ival<-1000, -1> || safe::ival<1, 1000>> not_zero
) {
    // ... do something really cool with this non-zero value ...
}
```

This makes it impossible to pass a value of `0` to this function. `safe::var`
can't be created or initialized with naked integral values, so how do we pass 
in parameters? We either need to use a `safe::var` that is already proven
to satisfy the callees requirements, or we can use `safe::function`:

```c++
bool fail = safe::function<void>(dont_give_me_zero)(0); // DOES NOT CALL FUNCTION
bool pass = safe::function<void>(dont_give_me_zero)(42); // CALLS FUNCTION
```

`safe::function` will check the values at runtime if necessary to prove they
satisfy the requirements of the function arguments. If any of them fail,
then the function is not called.

## Other Libraries

- [safe_numerics](https://github.com/boostorg/safe_numerics)
- [PSsimplesafeint](https://github.com/PeterSommerlad/PSsimplesafeint)
- [SafeInt](https://github.com/dcleblanc/SafeInt)
