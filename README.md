**Eggs.Tupleware**
==================

# Introduction #

**Eggs.Tupleware** is a **C++11** library to operate on `std::tuple`s, inspired
by [_Boost.Fusion_][boost.fusion] and [_Boost.MPL_][boost.mpl]. Unlike those 
libraries, there is no concept of _Sequence_ but only that of _Tuple-like_ 
type. A _Tuple-like_ type is one that fulfills the `tuple`-like protocol, 
examples of such types other than `std::tuple<...>` are `std::pair<T, U>` and 
`std::array<N, T>`.

[boost.fusion]: http://www.boost.org/doc/libs/release/libs/fusion/ "Boost.Fusion"
[boost.mpl]: http://www.boost.org/doc/libs/release/libs/mpl/ "Boost.MPL"

# Design decisions #

The library is implemented making extensive use of **C++11** features, and in 
a way which is transparent to features like `constexpr` and `noexcept` where 
possible. 

All functionality is provided within the `eggs::tupleware` namespace. For each 
provided function `f`, the following is also provided:

- `result_of::f<Args...>`, with a member typedef `type` equivalent to the
expression `INVOKE(f(std::declval<Args>()...))` when such expression is 
well-formed, otherwise there is no member `type`.

- `functional::f`, a function object that forwards to `f` when the call 
expression is well-formed.

- `meta::f`, a metaprogramming equivalent of `f`, only provided when the 
functionality has no side-effects.

Calls to the base functionality `f` will detect requirement violations (when 
the resulting error is a soft-error), and report them via `static_assert`s 
&mdash;which provides clear information on what's wrong but none as to why; 
more experimenting is needed in order to include the actual compiler error 
with reduced noise if possible&mdash;.

In order to propagate `constexpr` and `noexcept`, the underlying standard 
library implementation has to provide those guarantees for a few key functions
&mdash;`get`, `move`, `forward`, ...&mdash;, which is only required by the
**C++14** standard. Compile-time performance will be greatly improved if the 
implementation of `std::tuple<...>` is non-recursive &mdash;as the one shipped 
with [_libc++_][libcxx]&mdash;.

[libcxx]: http://libcxx.llvm.org/ "libc++ C++ Standard Library"

---

> Copyright _Agustín Bergé_, _Fusion Fenix_ 2014
> 
> Distributed under the Boost Software License, Version 1.0. (See accompanying
> file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

