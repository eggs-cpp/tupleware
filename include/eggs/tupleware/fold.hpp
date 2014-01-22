//! \file eggs/tupleware/fold.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_FOLD_HPP
#define EGGS_TUPLEWARE_FOLD_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/invoke.hpp>
#include <eggs/tupleware/is_tuple.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        //! \cond DETAIL
        namespace detail
        {
            template <
                typename Is
              , typename Tuple, typename F, typename State
              , typename Enable = void
            >
            struct fold_impl
            {};

            template <typename Tuple, typename F, typename State>
            struct fold_impl<
                tupleware::detail::index_sequence<>
              , Tuple, F, State
              , typename std::enable_if<is_tuple<Tuple>::value>::type
            >
            {
                using type = State;
            };

            template <
                std::size_t I, std::size_t ...Is
              , typename Tuple, typename F, typename State
            >
            struct fold_impl<
                tupleware::detail::index_sequence<I, Is...>
              , Tuple, F, State
              , typename std::enable_if<is_tuple<Tuple>::value>::type
            >
            {
                using type =
                    typename invoke<
                        F, State
                      , typename at<I, Tuple>::type
                    >::type;
            };
        }
        //! \endcond

        template <typename Tuple, typename UnaryMetaFunction, typename State>
        struct fold
          : detail::fold_impl<
                tupleware::detail::index_sequence_for_tuple<Tuple>
              , Tuple, UnaryMetaFunction, State
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
        template <typename Tuple, typename F, typename State>
        constexpr auto fold_impl(
            index_sequence<>
          , Tuple&& /*tuple*/, F& /*f*/, State&& state)
        EGGS_TUPLEWARE_AUTO_RETURN(
            std::forward<State>(state)
        )

        template <
            std::size_t I, std::size_t ...Is
          , typename Tuple, typename F, typename State
        >
        constexpr auto fold_impl(
            index_sequence<I, Is...>
          , Tuple&& tuple, F& f, State&& state)
        EGGS_TUPLEWARE_AUTO_RETURN(
            fold_impl(
                std::forward<Tuple>(tuple), f
              , invoke(f, std::forward<State>(state)
                  , at(meta::size_t<I>{}, std::forward<Tuple>(tuple))))
        )

        template <typename Tuple, typename F, typename State>
        constexpr auto fold(Tuple&& tuple, F f, State&& state)
        EGGS_TUPLEWARE_AUTO_RETURN(
            detail::fold_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple)
              , f, std::forward<State>(state))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_fold
          , ::eggs::tupleware::detail::fold
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename Tuple, typename UnaryFunction, typename State>
        struct fold
          : detail::_result_of_fold<pack<
                Tuple, UnaryFunction, State
            >>
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
    typename tupleware::detail::enable_if_failure<
        result_of::fold<Tuple, UnaryFunction, State>
    >::type fold(Tuple&& /*tuple*/, UnaryFunction&& /*f*/, State&& /*state*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::fold<Tuple, UnaryFunction, State>
            >::value
          , "ill-formed invoke expression");
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_FOLD_HPP*/
