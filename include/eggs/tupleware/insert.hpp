//! \file eggs/tupleware/insert.hpp
// Eggs.Tupleware
// 
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_INSERT_HPP
#define EGGS_TUPLEWARE_INSERT_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>
#include <eggs/tupleware/size.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        namespace detail
        {
            template <
                typename Is, typename Js
              , typename Tuple, typename Value
              , typename Enable = void
            >
            struct insert_impl
            {};

            template <
                std::size_t ...Is, std::size_t ...Js
              , typename Tuple, typename Value
            >
            struct insert_impl<
                tupleware::detail::index_sequence<Is...>
              , tupleware::detail::index_sequence<Js...>
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
                      , typename at<Js + sizeof...(Is), Tuple>::type...
                    >;
            };

            ///////////////////////////////////////////////////////////////////
            template <
                typename Is, typename Js, typename Ks
              , typename Tuple, typename Value
              , typename Enable = void
            >
            struct insert_expand_impl
            {};

            template <
                std::size_t ...Is, std::size_t ...Js, std::size_t ...Ks
              , typename Tuple, typename Value
            >
            struct insert_expand_impl<
                tupleware::detail::index_sequence<Is...>
              , tupleware::detail::index_sequence<Js...>
              , tupleware::detail::index_sequence<Ks...>
              , Tuple, Value
              , typename std::enable_if<
                    is_tuple<Tuple>::value && is_tuple<Value>::value
                >::type
            >
            {
                using type =
                    tupleware::tuple<
                        typename at<Is, Tuple>::type...
                      , typename at<Ks, Value>::type...
                      , typename at<Js + sizeof...(Is), Tuple>::type...
                    >;
            };
        }

        template <
            std::size_t Where
          , typename Tuple, typename Value, typename Expand = void
        >
        struct insert
          : detail::insert_impl<
                tupleware::detail::make_index_sequence<Where>
              , tupleware::detail::make_index_sequence<size<Tuple>::value - Where>
              , Tuple, Value
            >
        {};

        template <
            std::size_t Where
          , typename Tuple, typename Value
        >
        struct insert<Where, Tuple, expand_tuple, Value>
          : detail::insert_expand_impl<
                tupleware::detail::make_index_sequence<Where>
              , tupleware::detail::make_index_sequence<size<Tuple>::value - Where>
              , tupleware::detail::index_sequence_for_tuple<Value>
              , Tuple, Value
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <
            std::size_t ...Is, std::size_t ...Js
          , typename Tuple, typename Value
        >
        constexpr auto insert_impl(
            index_sequence<Is...>, index_sequence<Js...>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::insert<
                sizeof...(Is)
              , typename std::decay<Tuple>::type
              , typename detail::decay<Value>::type
            >::type{
                at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
              , std::forward<Value>(value)
              , at(meta::size_t<Js + sizeof...(Is)>{}, std::forward<Tuple>(tuple))...}
        )

        template <
            std::size_t Where
          , typename Tuple, typename Value
        >
        constexpr auto insert(
            meta::size_t<Where>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            insert_impl(
                make_index_sequence<Where>{}
              , make_index_sequence<result_of::size_t<Tuple>::value - Where>{}
              , std::forward<Tuple>(tuple), std::forward<Value>(value))
        )

        ///////////////////////////////////////////////////////////////////////
        template <
            std::size_t ...Is, std::size_t ...Js, std::size_t ...Ks
          , typename Tuple, typename Value
        >
        constexpr auto insert_expand_impl(
            index_sequence<Is...>, index_sequence<Js...>
          , index_sequence<Ks...>
          , Tuple&& tuple, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::insert<
                sizeof...(Is)
              , typename std::decay<Tuple>::type
              , meta::expand_tuple, typename std::decay<Value>::type
            >::type{
                at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...
              , at(meta::size_t<Ks>{}, std::forward<Value>(value))...
              , at(meta::size_t<Js + sizeof...(Is)>{}, std::forward<Tuple>(tuple))...}
        )

        template <
            std::size_t Where
          , typename Tuple, typename Value
        >
        constexpr auto insert(
            meta::size_t<Where>
          , Tuple&& tuple, expand_tuple_t, Value&& value)
        EGGS_TUPLEWARE_AUTO_RETURN(
            insert_expand_impl(
                make_index_sequence<Where>{}
              , make_index_sequence<result_of::size_t<Tuple>::value - Where>{}
              , index_sequence_for_tuple<Value>{}
              , std::forward<Tuple>(tuple), std::forward<Value>(value))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_insert
          , ::eggs::tupleware::detail::insert
        );
    }

    namespace result_of
    {
        template <
            std::size_t Where
          , typename Tuple, typename Value, typename Expand = void
        >
        struct insert
          : detail::_result_of_insert<pack<
                meta::size_t<Where>
              , Tuple, Value, Expand
            >>
        {};

        template <
            std::size_t Where
          , typename Tuple, typename Value
        >
        struct insert<Where, Tuple, Value>
          : detail::_result_of_insert<pack<
                meta::size_t<Where>
              , Tuple, Value
            >>
        {};

        template <
            std::size_t Where
          , typename Tuple, typename Value, typename Expand = void
        >
        using insert_t =
            typename insert<Where, Tuple, Value, Expand>::type;
    }

    template <std::size_t Where, typename Tuple, typename Value>
    constexpr result_of::insert_t<Where, Tuple, Value>
    insert(Tuple&& tuple, Value&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::insert(
            meta::size_t<Where>{}
          , std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    template <std::size_t Where, typename Tuple, typename Value>
    constexpr result_of::insert_t<Where, Tuple, expand_tuple_t, Value>
    insert(Tuple&& tuple, expand_tuple_t, Value&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::insert(
            meta::size_t<Where>{}
          , std::forward<Tuple>(tuple)
          , expand_tuple_t{}, std::forward<Value>(value))
    )

    namespace functional
    {
        template<std::size_t Where>
        struct insert
        {
            template <typename Tuple, typename Value>
            constexpr result_of::insert_t<Where, Tuple, Value>
            operator()(Tuple&& tuple, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::insert(
                    std::forward<Tuple>(tuple), std::forward<Value>(value))
            )

            template <typename Tuple, typename Value>
            constexpr result_of::insert_t<Where, Tuple, expand_tuple_t, Value>
            operator()(Tuple&& tuple, expand_tuple_t, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::insert(
                    std::forward<Tuple>(tuple)
                  , expand_tuple_t{}, std::forward<Value>(value))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t Where, typename Tuple, typename Value>
    typename tupleware::detail::enable_if_failure<
        result_of::insert<Where, Tuple, Value>
    >::type insert(Tuple&& /*tuple*/, Value&& /*value*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            Where < result_of::size_t<Tuple>::value
          , "'Where' argument is out of bounds");

        static_assert(
            tupleware::detail::inject_context<
                result_of::insert<Where, Tuple, Value>
            >::value
          , "invalid insert");
    }

    template <std::size_t Where, typename Tuple, typename Value>
    typename tupleware::detail::enable_if_failure<
        result_of::insert<Where, Tuple, expand_tuple_t, Value>
    >::type insert(Tuple&& /*tuple*/, expand_tuple_t, Value&& /*value*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            Where < result_of::size_t<Tuple>::value
          , "'Where' argument is out of bounds");

        static_assert(
            result_of::is_tuple_t<Value>::value
          , "'value' argument is not a Tuple");

        static_assert(
            tupleware::detail::inject_context<
                result_of::insert<Where, Tuple, expand_tuple_t, Value>
            >::value
          , "invalid insert");
    }
}}

#endif /*EGGS_TUPLEWARE_INSERT_HPP*/
