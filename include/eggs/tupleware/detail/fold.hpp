//! \file eggs/tupleware/detail/fold.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_FOLD_HPP
#define EGGS_TUPLEWARE_DETAIL_FOLD_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/invoke.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
{
    namespace meta
    {
        template <
            typename Is
          , typename Tuple, typename F, typename State
        >
        struct fold_impl
        {};

        template <typename Tuple, typename F, typename State>
        struct fold_impl<
            index_sequence<>
          , Tuple, F, State
        > : identity<State>
        {};

        template <
            std::size_t I, std::size_t ...Is
          , typename Tuple, typename F, typename State
        >
        struct fold_impl<
            index_sequence<I, Is...>
          , Tuple, F, State
        > : fold_impl<
                index_sequence<Is...>
              , Tuple, F
              , typename invoke<
                    F
                  , pack<State, typename at<I, Tuple>::type>
                >::type
            >
        {};
        
        ///////////////////////////////////////////////////////////////////////
        template <
            typename Tuple, typename F, typename State
          , typename Enable = void
        >
        struct fold
        {
            static_assert(
                is_tuple<Tuple>::value
              , "'Tuple' is not a tuple");
        };
        
        template <typename Tuple, typename F, typename State>
        struct fold<
            Tuple, F, State
          , typename std::enable_if<
                is_tuple<Tuple>::value
            >::type
        > : fold_impl<
                index_sequence_for_tuple<Tuple>
              , Tuple, F, State
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename F, typename State>
    constexpr auto fold(Tuple&& tuple, F f, State&& state)
    EGGS_TUPLEWARE_AUTO_RETURN(
        detail::fold_impl(
            index_sequence_for_tuple<Tuple>{}
          , std::forward<Tuple>(tuple)
          , f, std::forward<State>(state))
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_fold
      , ::eggs::tupleware::detail::fold
    );

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename UnaryFunction, typename State>
    void _explain_fold(Tuple&& tuple, UnaryFunction&& /*f*/, State&& /*state*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            inject_context<
                _result_of_fold<Tuple, UnaryFunction, State>
            >::value
          , "ill-formed invoke expression");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_FOLD_HPP*/
