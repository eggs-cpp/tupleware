//! \file eggs/tupleware/detail/is_tuple.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_IS_TUPLE_HPP
#define EGGS_TUPLEWARE_DETAIL_IS_TUPLE_HPP

#include <eggs/tupleware/core.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
{
    namespace meta
    {
        template <
            typename T
          , typename Enable = void
        >
        struct is_tuple
          : std::false_type
        {};

        template <typename T>
        struct is_tuple<T const>
          : std::false_type
        {};

        template <typename T>
        struct is_tuple<
            T
          , typename std::enable_if<
                (extension::tuple<T>::size >= 0)
            >::type
        > : std::true_type
        {};
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    constexpr auto is_tuple(meta::identity<T>)
    EGGS_TUPLEWARE_AUTO_RETURN(
        typename meta::is_tuple<typename std::decay<T>::type>::type{}
    )
    
    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_is_tuple
      , ::eggs::tupleware::detail::is_tuple
    );
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple>
    void _explain_is_tuple(Tuple&& /*tuple*/)
    {
        static_assert(
            _result_of_is_tuple<
                meta::identity<Tuple>
            >::type::value
          , "'tuple' argument is not a Tuple");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_IS_TUPLE_HPP*/
