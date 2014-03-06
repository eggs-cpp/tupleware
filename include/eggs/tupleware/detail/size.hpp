//! \file eggs/tupleware/detail/size.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_SIZE_HPP
#define EGGS_TUPLEWARE_DETAIL_SIZE_HPP

#include <eggs/tupleware/core.hpp>
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
            typename Tuple
          , typename Enable = void
        >
        struct size
        {
            static_assert(
                is_tuple<Tuple>::value
              , "'Tuple' is not a tuple");
        };

        template <typename Tuple>
        struct size<Tuple const>
        {
            static_assert(
                is_tuple<Tuple const>::value
              , "cv-qualified 'Tuple' is not a tuple");
        };

        template <typename Tuple>
        struct size<
            Tuple
          , typename std::enable_if<
                is_tuple<Tuple>::value
            >::type
        > : std::integral_constant<
                std::size_t
              , extension::tuple<Tuple>::size
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple>
    constexpr auto size(meta::identity<Tuple>)
    EGGS_TUPLEWARE_AUTO_RETURN(
        typename meta::size<typename std::decay<Tuple>::type>::type{}
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_size
      , ::eggs::tupleware::detail::size
    );

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple>
    void _explain_size(Tuple&& tuple)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_SIZE_HPP*/
