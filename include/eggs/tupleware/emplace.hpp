//! \file eggs/tupleware/emplace.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_EMPLACE_HPP
#define EGGS_TUPLEWARE_EMPLACE_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/insert.hpp>
#include <eggs/tupleware/is_tuple.hpp>
#include <eggs/tupleware/size.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace eggs { namespace tupleware
{
    //! \cond DETAIL
    namespace detail
    {
        template <
            typename T
          , std::size_t ...Is, std::size_t ...Js
          , typename Tuple, typename ...Args
        >
        constexpr auto emplace_impl(
            meta::identity<T>
          , index_sequence<Is...>, index_sequence<Js...>
          , Tuple&& tuple, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::insert<
                sizeof...(Is)
              , typename std::decay<Tuple>::type
              , typename std::remove_cv<T>::type
            >::type{
                at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
              , T(std::forward<Args>(args)...)
              , at(meta::size_t<Js + sizeof...(Is)>{}, std::forward<Tuple>(tuple))...}
        )

        template <
            std::size_t Where, typename T
          , typename Tuple, typename ...Args
        >
        constexpr auto emplace(
            meta::size_t<Where>, meta::identity<T>
          , Tuple&& tuple, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            emplace_impl(
                meta::identity<T>{}
              , make_index_sequence<Where>{}
              , make_index_sequence<result_of::size_t<Tuple>::value - Where>{}
              , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_emplace
          , ::eggs::tupleware::detail::emplace
        );
    }
    //! \endcond

    namespace result_of
    {
        template <
            std::size_t Where, typename T
          , typename Tuple, typename ...Args
        >
        struct emplace
          : detail::_result_of_emplace<pack<
                meta::size_t<Where>, meta::identity<T>
              , Tuple, Args...
            >>
        {};

        template <
            std::size_t Where, typename T
          , typename Tuple, typename ...Args
        >
        using emplace_t =
            typename emplace<Where, T, Tuple, Args...>::type;
    }

    template <std::size_t Where, typename T, typename Tuple, typename ...Args>
    constexpr result_of::emplace_t<Where, T, Tuple, Args...>
    emplace(Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_RETURN(
        detail::emplace(
            meta::size_t<Where>{}, meta::identity<T>{}
          , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
    )

    namespace functional
    {
        template<std::size_t Where, typename T>
        struct emplace
        {
            template <typename Tuple, typename ...Args>
            constexpr result_of::emplace_t<Where, T, Tuple, Args...>
            operator()(Tuple&& tuple, Args&&... args) const
            EGGS_TUPLEWARE_RETURN(
                detail::emplace(
                    meta::size_t<Where>{}, meta::identity<T>{}
                  , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t Where, typename T, typename Tuple, typename ...Args>
    typename tupleware::detail::enable_if_failure<
        result_of::emplace<Where, T, Tuple, Args...>
    >::type emplace(Tuple&& /*tuple*/, Args&&... /*args*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            Where < result_of::size_t<Tuple>::value
          , "'Where' argument is out of bounds");

        static_assert(
            tupleware::detail::inject_context<
                result_of::emplace<Where, T, Tuple, Args...>
            >::value
          , "invalid emplace");
    }
}}

#endif /*EGGS_TUPLEWARE_EMPLACE_HPP*/
