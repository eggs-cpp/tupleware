//! \file eggs/tupleware/for_each.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_FOR_EACH_HPP
#define EGGS_TUPLEWARE_FOR_EACH_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/invoke.hpp>
#include <eggs/tupleware/is_tuple.hpp>

#include <cstddef>
#include <utility>

namespace eggs { namespace tupleware
{
    //! \cond DETAIL
    namespace detail
    {
        template <
            std::size_t ...Is
          , typename Tuple, typename F
        >
        constexpr auto for_each_impl(
            index_sequence<Is...>
          , Tuple&& tuple, F& f)
        EGGS_TUPLEWARE_AUTO_RETURN(
            sequencer{
                (invoke(
                    f
                  , at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))), 0)...}
              , std::move(f)
        )

        template <typename Tuple, typename F>
        constexpr auto for_each(Tuple&& tuple, F f)
        EGGS_TUPLEWARE_AUTO_RETURN(
            for_each_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple), f)
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_for_each
          , ::eggs::tupleware::detail::for_each
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename Tuple, typename UnaryFunction>
        struct for_each
          : detail::_result_of_for_each<pack<
                Tuple, UnaryFunction
            >>
        {};

        template <typename Tuple, typename UnaryFunction>
        using for_each_t =
            typename for_each<Tuple, UnaryFunction>::type;
    }

    template <typename Tuple, typename UnaryFunction>
    constexpr result_of::for_each_t<Tuple, UnaryFunction>
    for_each(Tuple&& tuple, UnaryFunction&& f)
    EGGS_TUPLEWARE_RETURN(
        detail::for_each(
            std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
    )

    namespace functional
    {
        struct for_each
        {
            template <typename Tuple, typename UnaryFunction>
            constexpr result_of::for_each_t<Tuple, UnaryFunction>
            operator()(Tuple&& tuple, UnaryFunction&& f) const
            EGGS_TUPLEWARE_RETURN(
                detail::for_each(
                    std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple, typename UnaryFunction>
    typename tupleware::detail::enable_if_failure<
        result_of::for_each<Tuple, UnaryFunction>
    >::type for_each(Tuple&& /*tuple*/, UnaryFunction&& /*f*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::for_each<Tuple, UnaryFunction>
            >::value
          , "ill-formed invoke expression");
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_FOR_EACH_HPP*/
