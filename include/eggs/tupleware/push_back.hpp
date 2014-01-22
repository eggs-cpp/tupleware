//! \file eggs/tupleware/push_back.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_PUSH_BACK_HPP
#define EGGS_TUPLEWARE_PUSH_BACK_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>

#include <cstddef>
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
              , typename Tuple, typename Value
              , typename Enable = void
            >
            struct push_back_impl
            {};

            template <
                std::size_t ...Is
              , typename Tuple, typename Value
            >
            struct push_back_impl<
                tupleware::detail::index_sequence<Is...>
              , Tuple, Value
              , typename std::enable_if<
                    is_tuple<Tuple>::value
                >::type
            >
            {
                using type =
                    tupleware::tuple<
                        typename at<Is, Tuple>::type...
                      , Value
                    >;
            };

            ///////////////////////////////////////////////////////////////////
            template <
                typename Is, typename Js
              , typename Tuple, typename Value
              , typename Enable = void
            >
            struct push_back_expand_impl
            {};

            template <
                std::size_t ...Is, std::size_t ...Js
              , typename Tuple, typename Value
            >
            struct push_back_expand_impl<
                tupleware::detail::index_sequence<Is...>
              , tupleware::detail::index_sequence<Js...>
              , Tuple, Value
              , typename std::enable_if<
                    is_tuple<Tuple>::value && is_tuple<Value>::value
                >::type
            >
            {
                using type =
                    tupleware::tuple<
                        typename at<Is, Tuple>::type...
                      , typename at<Js, Value>::type...
                    >;
            };
        }
        //! \endcond

        template <typename Tuple, typename Value, typename Expand = void>
        struct push_back
          : detail::push_back_impl<
                tupleware::detail::index_sequence_for_tuple<Tuple>
              , Tuple, Value
            >
        {};

        template <typename Tuple, typename Value>
        struct push_back<Tuple, expand_tuple, Value>
          : detail::push_back_expand_impl<
                tupleware::detail::index_sequence_for_tuple<Tuple>
              , tupleware::detail::index_sequence_for_tuple<Value>
              , Tuple, Value
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
        template <
            std::size_t ...Is
          , typename Tuple, typename Value
        >
        constexpr auto push_back_impl(
            index_sequence<Is...>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::push_back<
                typename std::decay<Tuple>::type
              , typename detail::decay<Value>::type
            >::type{
                at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
              , std::forward<Value>(value)}
        )

        template <typename Tuple, typename Value>
        constexpr auto push_back(Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            push_back_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple), std::forward<Value>(value))
        )

        ///////////////////////////////////////////////////////////////////////
        template <
            std::size_t ...Is, std::size_t ...Js
          , typename Tuple, typename Value
        >
        constexpr auto push_back_expand_impl(
            index_sequence<Is...>, index_sequence<Js...>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::push_back<
                typename std::decay<Tuple>::type
              , meta::expand_tuple, typename std::decay<Value>::type
            >::type{
                at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
              , at(meta::size_t<Js>{}, std::forward<Value>(value))...}
        )

        template <typename Tuple, typename Value>
        constexpr auto push_back(Tuple&& tuple, expand_tuple_t, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            push_back_expand_impl(
                index_sequence_for_tuple<Tuple>{}
              , index_sequence_for_tuple<Value>{}
              , std::forward<Tuple>(tuple), std::forward<Value>(value))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_push_back
          , ::eggs::tupleware::detail::push_back
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename Tuple, typename Value, typename Expand = void>
        struct push_back
          : detail::_result_of_push_back<pack<
                Tuple, Value, Expand
            >>
        {};

        template <typename Tuple, typename Value>
        struct push_back<Tuple, Value>
          : detail::_result_of_push_back<pack<
                Tuple, Value
            >>
        {};

        template <typename Tuple, typename Value, typename Expand = void>
        using push_back_t =
            typename push_back<Tuple, Value, Expand>::type;
    }

    template <typename Tuple, typename Value>
    constexpr result_of::push_back_t<Tuple, Value>
    push_back(Tuple&& tuple, Value&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::push_back(
            std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    template <typename Tuple, typename Value>
    constexpr result_of::push_back_t<Tuple, expand_tuple_t, Value>
    push_back(Tuple&& tuple, expand_tuple_t, Value&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::push_back(
            std::forward<Tuple>(tuple)
          , expand_tuple_t{}, std::forward<Value>(value))
    )

    namespace functional
    {
        struct push_back
        {
            template <typename Tuple, typename Value>
            constexpr result_of::push_back_t<Tuple, Value>
            operator()(Tuple&& tuple, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::push_back(
                    std::forward<Tuple>(tuple), std::forward<Value>(value))
            )

            template <typename Tuple, typename Value>
            constexpr result_of::push_back_t<Tuple, expand_tuple_t, Value>
            operator()(Tuple&& tuple, expand_tuple_t, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::push_back(
                    std::forward<Tuple>(tuple)
                  , expand_tuple_t{}, std::forward<Value>(value))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple, typename Value>
    typename tupleware::detail::enable_if_failure<
        result_of::push_back<Tuple, Value>
    >::type push_back(Tuple&& /*tuple*/, Value&& /*value*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::push_back<Tuple, Value>
            >::value
          , "invalid push_back");
    }

    template <typename Tuple, typename Value>
    typename tupleware::detail::enable_if_failure<
        result_of::push_back<Tuple, expand_tuple_t, Value>
    >::type push_back(Tuple&& /*tuple*/, expand_tuple_t, Value&& /*value*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            result_of::is_tuple_t<Value>::value
          , "'value' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::push_back<Tuple, expand_tuple_t, Value>
            >::value
          , "invalid push_back");
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_PUSH_BACK_HPP*/
