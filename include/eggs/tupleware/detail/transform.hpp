//! \file eggs/tupleware/detail/transform.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_TRANSFORM_HPP
#define EGGS_TUPLEWARE_DETAIL_TRANSFORM_HPP

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
          , typename Tuple, typename F
        >
        struct transform_impl
        {};

        template <
            std::size_t ...Is
          , typename Tuple, typename F
        >
        struct transform_impl<
            index_sequence<Is...>
          , Tuple, F
        > : identity<
                tupleware::tuple<
                    typename invoke<
                        F
                      , pack<typename at<Is, Tuple>::type>
                    >::type...
                >
            >
        {};

        ///////////////////////////////////////////////////////////////////////
        template <
            typename Tuple, typename F
          , typename Enable = void
        >
        struct transform
        {
            static_assert(
                is_tuple<Tuple>::value
              , "'Tuple' is not a tuple");
        };

        template <typename Tuple, typename F>
        struct transform<
            Tuple, F
          , typename std::enable_if<
                is_tuple<Tuple>::value
            >::type
        > : transform_impl<
                index_sequence_for_tuple<Tuple>
              , Tuple, F
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    template <
        std::size_t ...Is
      , typename Tuple, typename F
    >
    constexpr auto transform_impl(
        index_sequence<Is...>
      , Tuple&& tuple, F& f)
    EGGS_TUPLEWARE_AUTO_RETURN(
        tupleware::tuple<decltype(
            invoke(f, at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))))...
        >{invoke(f, at(meta::size_t<Is>{}, std::forward<Tuple>(tuple)))...}
    )

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename F>
    constexpr auto transform(Tuple&& tuple, F f)
    EGGS_TUPLEWARE_AUTO_RETURN(
        transform_impl(
            index_sequence_for_tuple<Tuple>{}
          , std::forward<Tuple>(tuple), f)
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_transform
      , ::eggs::tupleware::detail::transform
    );

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename UnaryFunction>
    void _explain_transform(Tuple&& tuple, UnaryFunction&& /*f*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            inject_context<
                _result_of_transform<Tuple, UnaryFunction>
            >::value
          , "ill-formed invoke expression");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_TRANSFORM_HPP*/
