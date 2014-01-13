//! \file eggs/tupleware/size.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_SIZE_HPP
#define EGGS_TUPLEWARE_SIZE_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>

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
                typename Tuple
              , typename Enable = void
            >
            struct size_impl
            {};

            template <typename Tuple>
            struct size_impl<Tuple const>
            {};

            template <typename Tuple>
            struct size_impl<
                Tuple
              , typename std::enable_if<
                    (std::tuple_size<Tuple>::value >= 0)
                >::type
            > : std::integral_constant<
                    std::size_t
                  , std::tuple_size<Tuple>::value
                >
            {};
        }

        template <typename Tuple>
        struct size
          : detail::size_impl<Tuple>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename Tuple>
        constexpr auto size(Tuple&& /*tuple*/)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::size<typename std::decay<Tuple>::type>::type{}
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_size
          , ::eggs::tupleware::detail::size
        );
    }

    namespace result_of
    {
        template <typename Tuple>
        struct size
          : detail::_result_of_size<pack<
                Tuple
            >>
        {};

        template <typename Tuple>
        using size_t =
            typename size<Tuple>::type;
    }

    template <typename Tuple>
    constexpr result_of::size_t<Tuple>
    size(Tuple const& tuple)
    EGGS_TUPLEWARE_RETURN(
        detail::size(tuple)
    )

    namespace functional
    {
        struct size
        {
            template <typename Tuple>
            constexpr result_of::size_t<Tuple>
            operator()(Tuple&& tuple) const
            EGGS_TUPLEWARE_RETURN(
                detail::size(std::forward<Tuple>(tuple))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple>
    typename tupleware::detail::enable_if_failure<
        result_of::size<Tuple>
    >::type size(Tuple const& /*tuple*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");
    }
}}

#endif /*EGGS_TUPLEWARE_SIZE_HPP*/
