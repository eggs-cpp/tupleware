//! \file eggs/tupleware/at.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_AT_HPP
#define EGGS_TUPLEWARE_AT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>
#include <eggs/tupleware/size.hpp>

#include <cstddef>
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
                std::size_t I, typename Tuple
              , typename Enable = void
            >
            struct at_impl
            {};

            template <std::size_t I, typename Tuple>
            struct at_impl<I, Tuple const>
            {};

            template <std::size_t I, typename Tuple>
            struct at_impl<
                I, Tuple
              , tupleware::detail::always_void<
                    typename std::tuple_element<I, Tuple>::type
                >
            > : std::tuple_element<I, Tuple>
            {};
        }

        template <std::size_t I, typename Tuple>
        struct at
          : detail::at_impl<I, Tuple>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <std::size_t I, typename Tuple>
        constexpr auto at(meta::size_t<I>, Tuple&& tuple)
        EGGS_TUPLEWARE_AUTO_RETURN(
            std::get<I>(std::forward<Tuple>(tuple))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_at
          , ::eggs::tupleware::detail::at
        );
    }

    namespace result_of
    {
        template <std::size_t I, typename Tuple>
        struct at
          : detail::_result_of_at<pack<
                meta::size_t<I>, Tuple
            >>
        {};

        template <std::size_t I, typename Tuple>
        using at_t =
            typename at<I, Tuple>::type;
    }

    template <std::size_t I, typename Tuple>
    constexpr result_of::at_t<I, Tuple>
    at(Tuple&& tuple)
    EGGS_TUPLEWARE_RETURN(
        detail::at(meta::size_t<I>{}, std::forward<Tuple>(tuple))
    )

    namespace functional
    {
        template <std::size_t I>
        struct at
        {
            template <typename Tuple>
            constexpr result_of::at_t<I, Tuple>
            operator()(Tuple&& tuple) const
            EGGS_TUPLEWARE_RETURN(
                detail::at(meta::size_t<I>{}, std::forward<Tuple>(tuple))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t I, typename Tuple>
    typename tupleware::detail::enable_if_failure<
        result_of::at<I, Tuple>
    >::type at(Tuple&& /*tuple*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            I < result_of::size_t<Tuple>::value
          , "'I' argument is out of bounds");
    }
}}

#endif /*EGGS_TUPLEWARE_AT_HPP*/
