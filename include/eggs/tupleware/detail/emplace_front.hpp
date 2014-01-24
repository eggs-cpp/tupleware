//! \file eggs/tupleware/detail/emplace_front.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_EMPLACE_FRONT_HPP
#define EGGS_TUPLEWARE_DETAIL_EMPLACE_FRONT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>
#include <eggs/tupleware/detail/push_front.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
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

    ///////////////////////////////////////////////////////////////////////
    template <typename T, typename Tuple, typename ...Args>
    void _explain_emplace_front(Tuple&& tuple, Args&&... /*args*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            inject_context<
                _result_of_emplace_front<T, Tuple, Args...>
            >::value
          , "invalid emplace_front");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_EMPLACE_FRONT_HPP*/
