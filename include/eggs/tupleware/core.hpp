//! \file eggs/tupleware/core.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_CORE_HPP
#define EGGS_TUPLEWARE_CORE_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>

//! \page tuple_protocol tuple-like protocol
//!
//! The `tuple`-like protocol is a set of standard traits and function
//! overloads that allow operating on a type that is not `std::tuple<...>`,
//! but can conform to its access interface. Examples of such types are
//! `std::pair<T, U>` and `std::array<N, T>`.
//!
//! In order for a type `T` to support the `tuple`-like protocol, it has to
//! meet the following requirements:
//!
//! - Provide a specialization of [`std::tuple_size<T>`]
//! (http://en.cppreference.com/w/cpp/utility/tuple/tuple_size) with a
//! BaseCharacteristic of `std::integral_constant<Integral, Value>` where
//! `Value` is an instance of type `Integral` representing the number of
//! elements in `T`. Specializations for _cv-qualified_ `T`s forwarding to
//! the base trait are provided by the Standard Library.
//!
//! - Provide a specialization of [`std::tuple_element<I, T>`]
//! (http://en.cppreference.com/w/cpp/utility/tuple/tuple_element) with a 
//! nested typedef `type` naming the `I`th element of `T`, where indexing is
//! zero based. Specializations for _cv-qualified_ `T`s forwarding to
//! the base trait are provided by the Standard Library.
//!
//! - Provide overloads of [`std::get<I>`]
//! (http://en.cppreference.com/w/cpp/utility/tuple/get) where:
//!
//!   + `std::get<I>(T& t)` returns a reference to the `I`th element of `t`,
//!     where indexing is zero-based.
//!
//!   + `std::get<I>(T&& t)` is equivalent to
//!     `return std::forward<TI>(get<I>(t))` with `TI` being the `I`th element
//!     of `T`, where indexing is zero-based.
//!
//!   + `std::get<I>(T const& t)` returns a const reference to the `I`th
//!     element of `t`, where indexing is zero-based.

namespace eggs { namespace tupleware
{
    ///////////////////////////////////////////////////////////////////////////
    namespace meta
    {
        struct null {};

        template <typename T>
        struct identity
        {
            using type = T;
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace meta
    {
        template <bool Value>
        using bool_ =
            std::integral_constant<bool, Value>;

        template <char Value>
        using char_ =
            std::integral_constant<char, Value>;

        template <int Value>
        using int_ =
            std::integral_constant<int, Value>;

        template <long Value>
        using long_ =
            std::integral_constant<long, Value>;

        template <std::size_t Value>
        using size_t =
            std::integral_constant<std::size_t, Value>;
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace meta
    {
        namespace detail
        {
            template <std::size_t I>
            struct placeholder {};
        }

        namespace placeholders
        {
            using _  = detail::placeholder<0>;
            using _1 = detail::placeholder<1>;
            using _2 = detail::placeholder<2>;
            using _3 = detail::placeholder<3>;
            using _4 = detail::placeholder<4>;
            using _5 = detail::placeholder<5>;
            using _6 = detail::placeholder<6>;
            using _7 = detail::placeholder<7>;
            using _8 = detail::placeholder<8>;
            using _9 = detail::placeholder<9>;
        }
        using namespace placeholders;
    }

    ///////////////////////////////////////////////////////////////////////////
    struct expand_tuple_t {};

    namespace meta
    {
        using expand_tuple = expand_tuple_t;
    }

    static constexpr expand_tuple_t expand_tuple = {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename ...T>
    struct pack
    {};

    ///////////////////////////////////////////////////////////////////////////
    using std::tuple;

    using std::make_tuple;
    using std::tie;
    using std::forward_as_tuple;

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        struct access {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T>
        struct _always_void_impl
        {
            using type = void;
        };

        template <typename T>
        using always_void =
            typename _always_void_impl<T>::type;
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename Expr, typename T, typename Enable = void>
        struct _enable_if_failure_impl
        {
            using type = T;
        };

        template <typename Expr, typename T>
        struct _enable_if_failure_impl<
            Expr, T
          , always_void<typename Expr::type>
        >
        {};

        ///////////////////////////////////////////////////////////////////////
        template <typename Expr, typename T, typename Enable = void>
        struct _disable_if_failure_impl
        {};

        template <typename Expr, typename T>
        struct _disable_if_failure_impl<
            Expr, T
          , always_void<typename Expr::type>
        >
        {
            using type = T;
        };

        template <typename Expr, typename T = void>
        struct enable_if_failure
          : detail::_enable_if_failure_impl<Expr, T>
        {};

        template <typename Expr, typename T = void>
        struct disable_if_failure
          : detail::_disable_if_failure_impl<Expr, T>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename TD>
        struct _decay_impl
        {
            using type = TD;
        };

        template <typename T>
        struct _decay_impl<std::reference_wrapper<T>>
        {
            using type = T&;
        };

        template <typename T>
        struct decay
          : _decay_impl<typename std::decay<T>::type>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T, T ...I>
        struct integer_sequence
        {
            typedef T value_type;

            static constexpr size_t size() noexcept
            {
                 return sizeof...(I);
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename T, typename Left, typename Right>
        struct _make_integer_sequence_join;

        template <typename T, T... Left, T... Right>
        struct _make_integer_sequence_join<
            T
          , integer_sequence<T, Left...>
          , integer_sequence<T, Right...>
        >
        {
            typedef integer_sequence<
                T
              , Left...
              , (sizeof...(Left) + Right)...
            > type;
        };

        template <typename T, std::size_t N>
        struct _make_integer_sequence_impl;

        template <typename T>
        struct _make_integer_sequence_impl<T, 0>
        {
            typedef integer_sequence<T> type;
        };

        template <typename T>
        struct _make_integer_sequence_impl<T, 1>
        {
            typedef integer_sequence<T, 0> type;
        };

        template <typename T>
        struct _make_integer_sequence_impl<T, 2>
        {
            typedef integer_sequence<T, 0, 1> type;
        };

        template <typename T, std::size_t N>
        struct _make_integer_sequence_impl
        {
            typedef
                typename _make_integer_sequence_join<
                    T
                  , typename _make_integer_sequence_impl<T, N / 2>::type
                  , typename _make_integer_sequence_impl<T, N - N / 2>::type
                >::type
                type;
        };

        template <typename T, std::size_t N>
        using make_integer_sequence =
            typename detail::_make_integer_sequence_impl<T, N>::type;

        ///////////////////////////////////////////////////////////////////////
        template <std::size_t ...I>
        using index_sequence =
            integer_sequence<std::size_t, I...>;

        ///////////////////////////////////////////////////////////////////////
        template <std::size_t N>
        using make_index_sequence =
            make_integer_sequence<std::size_t, N>;

        ///////////////////////////////////////////////////////////////////////
        template <typename ...T>
        using index_sequence_for =
            make_index_sequence<sizeof...(T)>;

        ///////////////////////////////////////////////////////////////////////
        template <typename Tuple>
        using index_sequence_for_tuple =
            detail::make_index_sequence<
                std::tuple_size<typename std::decay<Tuple>::type>::value
            >;
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <bool Value>
        struct noexcept_bool
        {
            constexpr explicit noexcept_bool() noexcept(Value) {}
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        struct sequencer
        {
            template <typename ...T>
            constexpr explicit sequencer(T const&...) noexcept {}
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T, bool Value = false>
        struct inject_context
        {
            static constexpr bool const value = Value;
        };
    }

    ///////////////////////////////////////////////////////////////////////////
#   define EGGS_TUPLEWARE_AUTO_RETURN(...)                                    \
    noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__)                  \
    {                                                                         \
        return __VA_ARGS__;                                                   \
    }                                                                         \
    /**/

#   define EGGS_TUPLEWARE_RETURN(...)                                         \
    noexcept(noexcept(__VA_ARGS__))                                           \
    {                                                                         \
        return __VA_ARGS__;                                                   \
    }                                                                         \
    /**/

#   define EGGS_TUPLEWARE_RESULT_OF(Name, F)                                  \
    template <typename Args, typename Enable = void>                          \
    struct Name {};                                                           \
                                                                              \
    template <typename ...Args>                                               \
    struct Name<                                                              \
        tupleware::pack<Args...>                                              \
      , tupleware::detail::always_void<decltype(F(std::declval<Args>()...))>  \
    >                                                                         \
    {                                                                         \
        using type = decltype(F(std::declval<Args>()...));                    \
    }                                                                         \
    /**/
}}

#endif /*EGGS_TUPLEWARE_CORE_HPP*/
