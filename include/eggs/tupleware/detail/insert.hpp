//! \file eggs/tupleware/detail/insert.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_INSERT_HPP
#define EGGS_TUPLEWARE_DETAIL_INSERT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>
#include <eggs/tupleware/detail/size.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
{
    namespace meta
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
            index_sequence<Is...>, index_sequence<Js...>
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
            index_sequence<Is...>, index_sequence<Js...>, index_sequence<Ks...>
          , Tuple, Value
          , typename std::enable_if<
                is_tuple<Tuple>::value
             && is_tuple<Value>::value
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

        ///////////////////////////////////////////////////////////////////////
        template <
            std::size_t Where
          , typename Tuple, typename Value, typename Expand = void
        >
        struct insert
          : insert_impl<
                make_index_sequence<Where>
              , make_index_sequence<size<Tuple>::value - Where>
              , Tuple, Value
            >
        {};

        template <
            std::size_t Where
          , typename Tuple, typename Value
        >
        struct insert<Where, Tuple, meta::expand_tuple, Value>
          : insert_expand_impl<
                make_index_sequence<Where>
              , make_index_sequence<size<Tuple>::value - Where>
              , index_sequence_for_tuple<Value>
              , Tuple, Value
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////
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
          , make_index_sequence<size(meta::identity<Tuple>{}) - Where>{}
          , std::forward<Tuple>(tuple), std::forward<Value>(value))
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
          , make_index_sequence<size(meta::identity<Tuple>{}) - Where>{}
          , index_sequence_for_tuple<Value>{}
          , std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_insert
      , ::eggs::tupleware::detail::insert
    );

    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t Where, typename Tuple, typename Value>
    void _explain_insert(Tuple&& tuple, Value&& /*value*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            Where < _result_of_size<
                Tuple
            >::type::value
          , "'Where' argument is out of bounds");

        static_assert(
            inject_context<
                _result_of_insert<
                    meta::size_t<Where>
                  , Tuple, Value
                >
            >::value
          , "invalid insert");
    }

    template <std::size_t Where, typename Tuple, typename Value>
    void _explain_insert(Tuple&& tuple, expand_tuple_t, Value&& value)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            Where <= _result_of_size<Tuple>::value
          , "'Where' argument is out of bounds");

        _explain_is_tuple(std::forward<Value>(value));

        static_assert(
            inject_context<
                _result_of_insert<
                    meta::size_t<Where>
                  , Tuple, expand_tuple_t, Value
                >
            >::value
          , "invalid insert");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_INSERT_HPP*/
