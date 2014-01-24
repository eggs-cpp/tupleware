//! \file eggs/tupleware/emplace_front.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_EMPLACE_FRONT_HPP
#define EGGS_TUPLEWARE_EMPLACE_FRONT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/emplace_front.hpp>

namespace eggs { namespace tupleware
{
    namespace result_of
    {
        template <
            typename T
          , typename Tuple, typename ...Args
        >
        struct emplace_front
          : detail::_result_of_emplace_front<
                meta::identity<T>
              , Tuple, Args...
            >
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
    //! \cond DETAIL
    template <typename T, typename Tuple, typename ...Args>
    typename detail::enable_if_failure<
        result_of::emplace_front<T, Tuple, Args...>
    >::type emplace_front(Tuple&& tuple, Args&&... args)
    {
        detail::_explain_emplace_front<T>(
            std::forward<Tuple>(tuple), std::forward<Args>(args)...);
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_EMPLACE_FRONT_HPP*/
