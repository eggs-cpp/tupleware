//! \file eggs/tupleware/emplace_back.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_EMPLACE_BACK_HPP
#define EGGS_TUPLEWARE_EMPLACE_BACK_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>
#include <eggs/tupleware/push_back.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace eggs { namespace tupleware
{
    namespace detail
    {
        template <
            typename T
          , std::size_t ...Is
          , typename Tuple, typename ...Args
        >
        constexpr auto emplace_back_impl(
            meta::identity<T>
          , index_sequence<Is...>
          , Tuple&& tuple, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::push_back<
                typename std::decay<Tuple>::type
              , typename std::remove_cv<T>::type
            >::type{
                at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
              , T(std::forward<Args>(args)...)}
        )

        template<
            typename T
          , typename Tuple, typename ...Args
        >
        constexpr auto emplace_back(
            meta::identity<T>
          , Tuple&& tuple, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            emplace_back_impl(
                meta::identity<T>{}
              , index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_emplace_back
          , ::eggs::tupleware::detail::emplace_back
        );
    }

    namespace result_of
    {
        template <typename T, typename Tuple, typename ...Args>
        struct emplace_back
          : detail::_result_of_emplace_back<pack<
                meta::identity<T>
              , Tuple, Args...
            >>
        {};

        template <typename T, typename Tuple, typename ...Args>
        using emplace_back_t =
            typename emplace_back<T, Tuple, Args...>::type;
    }

    template <typename T, typename Tuple, typename ...Args>
    constexpr result_of::emplace_back_t<T, Tuple, Args...>
    emplace_back(Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_RETURN(
        detail::emplace_back(
            meta::identity<T>{}
          , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
    )

    namespace functional
    {
        template <typename T>
        struct emplace_back
        {
            template <typename Tuple, typename ...Args>
            constexpr result_of::emplace_back_t<T, Tuple, Args...>
            operator()(Tuple&& tuple, Args&&... args) const
            EGGS_TUPLEWARE_RETURN(
                detail::emplace_back(
                    meta::identity<T>{}
                  , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Tuple, typename ...Args>
    typename tupleware::detail::enable_if_failure<
        result_of::emplace_back<T, Tuple, Args...>
    >::type emplace_back(Tuple&& /*tuple*/, Args&&... /*args*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::emplace_back<T, Tuple, Args...>
            >::value
          , "invalid emplace_back");
    }
}}

#endif /*EGGS_TUPLEWARE_EMPLACE_BACK_HPP*/
