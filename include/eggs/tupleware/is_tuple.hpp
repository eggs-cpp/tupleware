// Copyright (c) 2014 Agustin K-ballo Berge
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_IS_TUPLE_HPP
#define EGGS_TUPLEWARE_IS_TUPLE_HPP

#include <eggs/tupleware/core.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        namespace detail
        {
            template <
                typename T
              , typename Enable = void
            >
            struct is_tuple_impl
              : std::false_type
            {};

            template <typename T>
            struct is_tuple_impl<T const>
              : std::false_type
            {};

            template <typename T>
            struct is_tuple_impl<
                T
              , typename std::enable_if<
                    (std::tuple_size<T>::value >= 0)
                >::type
            > : std::true_type
            {};
        }

        template <typename T>
        struct is_tuple
          : detail::is_tuple_impl<T>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        template <typename T>
        struct is_tuple
          : meta::is_tuple<typename std::decay<T>::type>
        {};

        template <typename T>
        using is_tuple_t =
            typename is_tuple<T>::type;
    }

    template <typename T>
    constexpr auto is_tuple() noexcept
     -> result_of::is_tuple_t<T>
    {
        return {};
    }

    template <typename T>
    constexpr auto is_tuple(T&& /*tuple*/) noexcept
     -> result_of::is_tuple_t<T>
    {
        return {};
    }

    namespace functional
    {
        struct is_tuple
        {
            template <typename Tuple>
            constexpr auto operator()(Tuple&& tuple) const
                noexcept(noexcept(tupleware::is_tuple(
                    std::forward<Tuple>(tuple))))
             -> result_of::is_tuple_t<Tuple>
            {
                return tupleware::is_tuple(
                    std::forward<Tuple>(tuple));
            }
        };
    }
}}

#endif /*EGGS_TUPLEWARE_IS_TUPLE_HPP*/
