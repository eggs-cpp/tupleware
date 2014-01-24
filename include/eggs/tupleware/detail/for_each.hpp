//! \file eggs/tupleware/detail/for_each.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_FOR_EACH_HPP
#define EGGS_TUPLEWARE_DETAIL_FOR_EACH_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/invoke.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>

#include <cstddef>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
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

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename F>
    constexpr auto for_each(Tuple&& tuple, F f)
    EGGS_TUPLEWARE_AUTO_RETURN(
        for_each_impl(
            index_sequence_for_tuple<Tuple>{}
          , std::forward<Tuple>(tuple), f)
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_for_each
      , ::eggs::tupleware::detail::for_each
    );

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename UnaryFunction>
    void _explain_for_each(Tuple&& tuple, UnaryFunction&& /*f*/)
    {
        _explain_is_tuple(std::forward(tuple));

        static_assert(
            inject_context<
                _result_of_for_each<Tuple, UnaryFunction>
            >::value
          , "ill-formed invoke expression");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_FOR_EACH_HPP*/
