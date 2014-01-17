//! \file eggs/tupleware/emplace_front.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_EMPLACE_FRONT_HPP
#define EGGS_TUPLEWARE_EMPLACE_FRONT_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>
#include <eggs/tupleware/push_front.hpp>

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
          , std::size_t ...Is
          , typename Tuple, typename ...Args
        >
        constexpr auto emplace_front_impl(
            meta::identity<T>
          , index_sequence<Is...>
          , Tuple&& tuple, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::push_front<
                typename std::remove_cv<T>::type
              , typename std::decay<Tuple>::type
            >::type{
                T(std::forward<Args>(args)...)
              , at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...}
        )

        template<
            typename T
          , typename Tuple, typename ...Args
        >
        constexpr auto emplace_front(
            meta::identity<T>
          , Tuple&& tuple, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            emplace_front_impl(
                meta::identity<T>{}
              , index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_emplace_front
          , ::eggs::tupleware::detail::emplace_front
        );
    }
    //! \endcond

    namespace result_of
    {
        template <
            typename T
          , typename Tuple, typename ...Args
        >
        struct emplace_front
          : detail::_result_of_emplace_front<pack<
                meta::identity<T>
              , Tuple, Args...
            >>
        {};

        template <
            typename T
          , typename Tuple, typename ...Args
        >
        using emplace_front_t =
            typename emplace_front<T, Tuple, Args...>::type;
    }

    template <typename T, typename Tuple, typename ...Args>
    constexpr result_of::emplace_front_t<T, Tuple, Args...>
    emplace_front(Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_RETURN(
        detail::emplace_front(
            meta::identity<T>{}
          , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
    )

    namespace functional
    {
        template <typename T>
        struct emplace_front
        {
            template <typename Tuple, typename ...Args>
            constexpr result_of::emplace_front_t<T, Tuple, Args...>
            operator()(Tuple&& tuple, Args&&... args) const
            EGGS_TUPLEWARE_RETURN(
                detail::emplace_front(
                    meta::identity<T>{}
                  , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Tuple, typename ...Args>
    typename tupleware::detail::enable_if_failure<
        result_of::emplace_front<T, Tuple, Args...>
    >::type emplace_front(Tuple&& /*tuple*/, Args&&... /*args*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::emplace_front<T, Tuple, Args...>
            >::value
          , "invalid emplace_front");
    }
}}

#endif /*EGGS_TUPLEWARE_EMPLACE_FRONT_HPP*/
