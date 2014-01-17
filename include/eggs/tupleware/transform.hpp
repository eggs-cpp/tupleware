//! \file eggs/tupleware/transform.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_TRANSFORM_HPP
#define EGGS_TUPLEWARE_TRANSFORM_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/invoke.hpp>
#include <eggs/tupleware/is_tuple.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        //! \cond DETAIL
        namespace detail
        {
            template <
                typename Is
              , typename Tuple, typename F
              , typename Enable = void
            >
            struct transform_impl
            {};

            template <
                std::size_t ...Is
              , typename Tuple, typename F
            >
            struct transform_impl<
                tupleware::detail::index_sequence<Is...>
              , Tuple, F
              , typename std::enable_if<is_tuple<Tuple>::value>::type
            >
            {
                using type =
                    tupleware::tuple<
                        typename invoke<F, typename at<Is, Tuple>::type>::type...
                    >;
            };
        }
        //! \endcond

        template <typename Tuple, typename UnaryMetaFunction>
        struct transform
          : detail::transform_impl<
                tupleware::detail::index_sequence_for_tuple<Tuple>
              , Tuple, UnaryMetaFunction
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
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

        template <typename Tuple, typename F>
        constexpr auto transform(Tuple&& tuple, F f)
        EGGS_TUPLEWARE_AUTO_RETURN(
            transform_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple), f)
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_transform
          , ::eggs::tupleware::detail::transform
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename Tuple, typename UnaryFunction>
        struct transform
          : detail::_result_of_transform<pack<
                Tuple, UnaryFunction
            >>
        {};

        template <typename Tuple, typename UnaryFunction>
        using transform_t =
            typename transform<Tuple, UnaryFunction>::type;
    }

    template <typename Tuple, typename UnaryFunction>
    constexpr result_of::transform_t<Tuple, UnaryFunction>
    transform(Tuple&& tuple, UnaryFunction&& f)
    EGGS_TUPLEWARE_RETURN(
        detail::transform(
            std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
    )

    namespace functional
    {
        struct transform
        {
            template <typename Tuple, typename UnaryFunction>
            constexpr result_of::transform_t<Tuple, UnaryFunction>
            operator()(Tuple&& tuple, UnaryFunction&& f) const
            EGGS_TUPLEWARE_RETURN(
                detail::transform(
                    std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename UnaryFunction>
    typename tupleware::detail::enable_if_failure<
        result_of::transform<Tuple, UnaryFunction>
    >::type transform(Tuple&& /*tuple*/, UnaryFunction&& /*f*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::transform<Tuple, UnaryFunction>
            >::value
          , "ill-formed invoke expression");
    }
}}

#endif /*EGGS_TUPLEWARE_TRANSFORM_HPP*/
