//! \file eggs/tupleware/detail/emplace_back.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_EMPLACE_BACK_HPP
#define EGGS_TUPLEWARE_DETAIL_EMPLACE_BACK_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>
#include <eggs/tupleware/detail/push_back.hpp>

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
    constexpr auto emplace_back_impl(
        meta::identity<T>
      , index_sequence<Is...>
      , Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_AUTO_RETURN(
        typename meta::push_back<
            typename std::decay<Tuple>::type
          , typename std::remove_cv<T>::type
        >::type{
            at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
          , T(std::forward<Args>(args)...)}
    )

    template<
        typename T
      , typename Tuple, typename ...Args
    >
    constexpr auto emplace_back(
        meta::identity<T>
      , Tuple&& tuple, Args&&... args)
    EGGS_TUPLEWARE_AUTO_RETURN(
        emplace_back_impl(
            meta::identity<T>{}
          , index_sequence_for_tuple<Tuple>{}
          , std::forward<Tuple>(tuple), std::forward<Args>(args)...)
    )

    ///////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_emplace_back
      , ::eggs::tupleware::detail::emplace_back
    );

    ///////////////////////////////////////////////////////////////////////
    template <typename T, typename Tuple, typename ...Args>
    void _explain_emplace_back(Tuple&& tuple, Args&&... /*args*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            inject_context<
                _result_of_emplace_back<T, Tuple, Args...>
            >::value
          , "invalid emplace_back");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_EMPLACE_BACK_HPP*/
