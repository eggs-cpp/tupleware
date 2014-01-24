//! \file eggs/tupleware/emplace.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_EMPLACE_HPP
#define EGGS_TUPLEWARE_EMPLACE_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/emplace.hpp>

namespace eggs { namespace tupleware
{
    namespace result_of
    {
        template <
            std::size_t Where, typename T
          , typename Tuple, typename ...Args
        >
        struct emplace
          : detail::_result_of_emplace<
                meta::size_t<Where>, meta::identity<T>
              , Tuple, Args...
            >
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
    //! \cond DETAIL
    template <std::size_t Where, typename T, typename Tuple, typename ...Args>
    typename detail::enable_if_failure<
        result_of::emplace<Where, T, Tuple, Args...>
    >::type emplace(Tuple&& tuple, Args&&... args)
    {
        detail::_explain_emplace<Where, T>(
            std::forward<Tuple>(tuple), std::forward<Args>(args)...);
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_EMPLACE_HPP*/
