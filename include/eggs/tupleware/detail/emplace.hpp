//! \file eggs/tupleware/detail/emplace.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_EMPLACE_HPP
#define EGGS_TUPLEWARE_DETAIL_EMPLACE_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/insert.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>
#include <eggs/tupleware/detail/size.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
{
    template <
        typename T
      , std::size_t ...Is, std::size_t ...Js
      , typename Tuple, typename ...Args
    >
    constexpr auto emplace_impl(
        meta::identity<T>
      , index_sequence<Is...>, index_sequence<Js...>
      , Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_AUTO_RETURN(
        typename meta::insert<
            sizeof...(Is)
          , typename std::decay<Tuple>::type
          , typename std::remove_cv<T>::type
        >::type{
            at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
          , T(std::forward<Args>(args)...)
          , at(meta::size_t<Js + sizeof...(Is)>{}, std::forward<Tuple>(tuple))...}
    )

    template <
        std::size_t Where, typename T
      , typename Tuple, typename ...Args
    >
    constexpr auto emplace(
        meta::size_t<Where>, meta::identity<T>
      , Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_AUTO_RETURN(
        emplace_impl(
            meta::identity<T>{}
          , make_index_sequence<Where>{}
          , make_index_sequence<size(meta::identity<Tuple>{}) - Where>{}
          , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
    )

    ///////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_emplace
      , ::eggs::tupleware::detail::emplace
    );

    ///////////////////////////////////////////////////////////////////////
    template <std::size_t Where, typename T, typename Tuple, typename ...Args>
    void _explain_emplace(Tuple&& tuple, Args&&... /*args*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            Where <= _result_of_size<Tuple>::value
          , "'Where' argument is out of bounds");

        static_assert(
            inject_context<
                _result_of_emplace<
                    meta::size_t<Where>
                  , T, Tuple, Args...
                >
            >::value
          , "invalid emplace");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_EMPLACE_HPP*/
