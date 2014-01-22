//! \file eggs/tupleware/push_front.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_PUSH_FRONT_HPP
#define EGGS_TUPLEWARE_PUSH_FRONT_HPP

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
            struct push_front_impl
            {};

            template <
                std::size_t ...Is
              , typename Tuple, typename Value
            >
            struct push_front_impl<
                tupleware::detail::index_sequence<Is...>
              , Tuple, Value
              , typename std::enable_if<
                    is_tuple<Tuple>::value
                >::type
            >
            {
                using type =
                    tupleware::tuple<
                        Value
                      , typename at<Is, Tuple>::type...
                    >;
            };

            ///////////////////////////////////////////////////////////////////
            template <
                typename Is, typename Js
              , typename Tuple, typename Value
              , typename Enable = void
            >
            struct push_front_expand_impl
            {};

            template <
                std::size_t ...Is, std::size_t ...Js
              , typename Tuple, typename Value
            >
            struct push_front_expand_impl<
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
                        typename at<Js, Value>::type...
                      , typename at<Is, Tuple>::type...
                    >;
            };
        }
        //! \endcond

        template <typename Tuple, typename Value, typename Expand = void>
        struct push_front
          : detail::push_front_impl<
                tupleware::detail::index_sequence_for_tuple<Tuple>
              , Tuple, Value
            >
        {};

        template <typename Tuple, typename Value>
        struct push_front<Tuple, expand_tuple, Value>
          : detail::push_front_expand_impl<
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
        constexpr auto push_front_impl(
            index_sequence<Is...>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::push_front<
                typename detail::decay<Value>::type
              , typename std::decay<Tuple>::type
            >::type{
                std::forward<Value>(value)
              , at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...}
        )

        template <typename Tuple, typename Value>
        constexpr auto push_front(Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            push_front_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple), std::forward<Value>(value))
        )

        ///////////////////////////////////////////////////////////////////////
        template <
            std::size_t ...Is, std::size_t ...Js
          , typename Tuple, typename Value
        >
        constexpr auto push_front_expand_impl(
            index_sequence<Is...>, index_sequence<Js...>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::push_front<
                typename std::decay<Tuple>::type
              , meta::expand_tuple, typename std::decay<Value>::type
            >::type{
                at(meta::size_t<Js>{}, std::forward<Value>(value))...
              , at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...}
        )

        template <typename Tuple, typename Value>
        constexpr auto push_front(Tuple&& tuple, expand_tuple_t, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            push_front_impl(
                index_sequence_for_tuple<Tuple>{}
              , index_sequence_for_tuple<Value>{}
              , std::forward<Tuple>(tuple), std::forward<Value>(value))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_push_front
          , ::eggs::tupleware::detail::push_front
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename Tuple, typename Value, typename Expand = void>
        struct push_front
          : detail::_result_of_push_front<pack<
                Tuple, Value, Expand
            >>
        {};

        template <typename Tuple, typename Value>
        struct push_front<Tuple, Value>
          : detail::_result_of_push_front<pack<
                Tuple, Value
            >>
        {};

        template <typename Tuple, typename Value, typename Expand = void>
        using push_front_t =
            typename push_front<Tuple, Value, Expand>::type;
    }

    template <typename Tuple, typename Value>
    constexpr result_of::push_front_t<Tuple, Value>
    push_front(Tuple&& tuple, Value&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::push_front(
            std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    template <typename Tuple, typename Value>
    constexpr result_of::push_front_t<Tuple, expand_tuple_t, Value>
    push_front(Tuple&& tuple, expand_tuple_t, Value&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::push_front(
            std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    namespace functional
    {
        struct push_front
        {
            template <typename Tuple, typename Value>
            constexpr result_of::push_front_t<Tuple, Value>
            operator()(Tuple&& tuple, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::push_front(
                    std::forward<Tuple>(tuple), std::forward<Value>(value))
            )

            template <typename Tuple, typename Value>
            constexpr result_of::push_front_t<Tuple, expand_tuple_t, Value>
            operator()(Tuple&& tuple, expand_tuple_t, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::push_front(
                    std::forward<Tuple>(tuple), std::forward<Value>(value))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple, typename Value>
    typename tupleware::detail::enable_if_failure<
        result_of::push_front<Tuple, Value>
    >::type push_front(Tuple&& /*tuple*/, Value&& /*value*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::push_front<Tuple, Value>
            >::value
          , "invalid push_front");
    }

    template <typename Tuple, typename Value>
    typename tupleware::detail::enable_if_failure<
        result_of::push_front<Tuple, expand_tuple_t, Value>
    >::type push_front(Tuple&& /*tuple*/, expand_tuple_t, Value&& /*value*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            result_of::is_tuple_t<Value>::value
          , "'value' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::push_front<Tuple, expand_tuple_t, Value>
            >::value
          , "invalid push_front");
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_PUSH_FRONT_HPP*/
