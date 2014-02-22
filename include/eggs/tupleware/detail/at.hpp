//! \file eggs/tupleware/detail/at.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_AT_HPP
#define EGGS_TUPLEWARE_DETAIL_AT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>
#include <eggs/tupleware/detail/size.hpp>

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
            std::size_t I, typename Tuple
          , typename Enable = void
        >
        struct at;

        template <std::size_t I, typename Tuple>
        struct at<I, Tuple const>;

        template <std::size_t I, typename Tuple>
        struct at<
            I, Tuple
          , typename std::enable_if<
                is_tuple<Tuple>::value
             && (I < size<Tuple>::value)
            >::type
        > : identity<
                typename extension::tuple<Tuple>::
                    template element<I>::type
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    template <
        std::size_t I
      , typename Tuple
      , typename Enable =
            typename std::enable_if<
                is_tuple(meta::identity<Tuple>{})
             && (I < size(meta::identity<Tuple>{}))
            >::type
    >
    constexpr auto at(meta::size_t<I>, Tuple&& tuple)
    EGGS_TUPLEWARE_AUTO_RETURN(
        extension::tuple<typename std::decay<Tuple>::type>::
            template element<I>::get(std::forward<Tuple>(tuple))
    )
    
    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_at
      , ::eggs::tupleware::detail::at
    );
    
    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t I, typename Tuple>
    void _explain_at(Tuple&& tuple)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            I < _result_of_size<
                meta::identity<Tuple>
            >::type::value
          , "'I' argument is out of bounds");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_AT_HPP*/
