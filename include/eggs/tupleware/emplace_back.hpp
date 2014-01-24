//! \file eggs/tupleware/emplace_back.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_EMPLACE_BACK_HPP
#define EGGS_TUPLEWARE_EMPLACE_BACK_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/emplace_back.hpp>

namespace eggs { namespace tupleware
{
    namespace result_of
    {
        template <typename T, typename Tuple, typename ...Args>
        struct emplace_back
          : detail::_result_of_emplace_back<
                meta::identity<T>
              , Tuple, Args...
            >
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
    //! \cond DETAIL
    template <typename T, typename Tuple, typename ...Args>
    typename detail::enable_if_failure<
        result_of::emplace_back<T, Tuple, Args...>
    >::type emplace_back(Tuple&& tuple, Args&&... args)
    {
        detail::_explain_emplace_back<T>(
            std::forward<Tuple>(tuple), std::forward<Args>(args)...);
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_EMPLACE_BACK_HPP*/
