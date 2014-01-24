//! \file eggs/tupleware/fold.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_FOLD_HPP
#define EGGS_TUPLEWARE_FOLD_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/fold.hpp>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        template <typename Tuple, typename UnaryMetaFunction, typename State>
        struct fold
          : detail::meta::fold<Tuple, UnaryMetaFunction, State>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        template <typename Tuple, typename UnaryFunction, typename State>
        struct fold
          : detail::_result_of_fold<
                Tuple, UnaryFunction, State
            >
        {};

        template <typename Tuple, typename UnaryFunction, typename State>
        using fold_t =
            typename fold<Tuple, UnaryFunction, State>::type;
    }

    template <typename Tuple, typename UnaryFunction, typename State>
    constexpr result_of::fold_t<Tuple, UnaryFunction, State>
    fold(Tuple&& tuple, UnaryFunction&& f, State&& state)
    EGGS_TUPLEWARE_RETURN(
        detail::fold(
            std::forward<Tuple>(tuple)
          , std::forward<UnaryFunction>(f), std::forward<State>(state))
    )

    namespace functional
    {
        struct fold
        {
            template <typename Tuple, typename UnaryFunction, typename State>
            constexpr result_of::fold_t<Tuple, UnaryFunction, State>
            operator()(Tuple&& tuple, UnaryFunction&& f, State&& state) const
            EGGS_TUPLEWARE_RETURN(
                detail::fold(
                    std::forward<Tuple>(tuple)
                  , std::forward<UnaryFunction>(f), std::forward<State>(state))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple, typename UnaryFunction, typename State>
    typename detail::enable_if_failure<
        result_of::fold<Tuple, UnaryFunction, State>
    >::type fold(Tuple&& tuple, UnaryFunction&& f, State&& state)
    {
        detail::_explain_fold(
            std::forward<Tuple>(tuple)
          , std::forward<UnaryFunction>(f), std::forward<State>(state));
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_FOLD_HPP*/
