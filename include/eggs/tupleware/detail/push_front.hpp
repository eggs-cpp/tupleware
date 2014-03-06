//! \file eggs/tupleware/detail/push_front.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_PUSH_FRONT_HPP
#define EGGS_TUPLEWARE_DETAIL_PUSH_FRONT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/at.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

//! \cond DETAIL
namespace eggs { namespace tupleware { namespace detail
{
    namespace meta
    {
        template <
            typename Is
          , typename Tuple, typename Value
        >
        struct push_front_impl
        {};

        template <
            std::size_t ...Is
          , typename Tuple, typename Value
        >
        struct push_front_impl<
            index_sequence<Is...>
          , Tuple, Value
        >
        {
            using type =
                tupleware::tuple<
                    Value
                  , typename at<Is, Tuple>::type...
                >;
        };

        ///////////////////////////////////////////////////////////////////////
        template <
            typename Is, typename Js
          , typename Tuple, typename Value
        >
        struct push_front_expand_impl
        {};

        template <
            std::size_t ...Is, std::size_t ...Js
          , typename Tuple, typename Value
        >
        struct push_front_expand_impl<
            index_sequence<Is...>, index_sequence<Js...>
          , Tuple, Value
        >
        {
            using type =
                tupleware::tuple<
                    typename at<Js, Value>::type...
                  , typename at<Is, Tuple>::type...
                >;
        };

        ///////////////////////////////////////////////////////////////////////
        template <
            typename Tuple, typename Value, typename Expand = void
          , typename Enable = void
        >
        struct push_front
        {
            static_assert(
                is_tuple<Tuple>::value
              , "'Tuple' is not a tuple");
            
            static_assert(
                !std::is_same<Value, meta::expand_tuple>::value
             || is_tuple<Expand>::value
              , "'Expand' is not a tuple");
        };

        template <typename Tuple, typename Value>
        struct push_front<
            Tuple, Value, void
          , typename std::enable_if<
                is_tuple<Tuple>::value
            >::type
        > : push_front_impl<
                index_sequence_for_tuple<Tuple>
              , Tuple, Value
            >
        {};

        template <typename Tuple, typename Value>
        struct push_front<
            Tuple, meta::expand_tuple, Value
          , typename std::enable_if<
                is_tuple<Tuple>::value
             && is_tuple<Value>::value
            >::type
        > : push_front_expand_impl<
                index_sequence_for_tuple<Tuple>
              , index_sequence_for_tuple<Value>
              , Tuple, Value
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename Value>
    constexpr auto push_front(Tuple&& tuple, Value&& value)
    EGGS_TUPLEWARE_AUTO_RETURN(
        push_front_impl(
            index_sequence_for_tuple<Tuple>{}
          , std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    template <typename Tuple, typename Value>
    constexpr auto push_front(Tuple&& tuple, expand_tuple_t, Value&& value)
    EGGS_TUPLEWARE_AUTO_RETURN(
        push_front_impl(
            index_sequence_for_tuple<Tuple>{}
          , index_sequence_for_tuple<Value>{}
          , std::forward<Tuple>(tuple), std::forward<Value>(value))
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_push_front
      , ::eggs::tupleware::detail::push_front
    );

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple, typename Value>
    void _explain_push_front(Tuple&& tuple, Value&& /*value*/)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        static_assert(
            inject_context<
                _result_of_push_front<Tuple, Value>
            >::value
          , "invalid push_front");
    }

    template <typename Tuple, typename Value>
    void _explain_push_front(Tuple&& tuple, expand_tuple_t, Value&& value)
    {
        _explain_is_tuple(std::forward<Tuple>(tuple));

        _explain_is_tuple(std::forward<Value>(value));

        static_assert(
            inject_context<
                _result_of_push_front<Tuple, expand_tuple_t, Value>
            >::value
          , "invalid push_front");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_PUSH_FRONT_HPP*/
