//! \file eggs/tupleware/push_front.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_PUSH_FRONT_HPP
#define EGGS_TUPLEWARE_PUSH_FRONT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/push_front.hpp>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        template <typename Tuple, typename Value, typename Expand = void>
        struct push_front
          : detail::meta::push_front<Tuple, Value, Expand>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        template <typename Tuple, typename Value, typename Expand = void>
        struct push_front
          : detail::_result_of_push_front<
                Tuple, Value, Expand
            >
        {};

        template <typename Tuple, typename Value>
        struct push_front<Tuple, Value>
          : detail::_result_of_push_front<
                Tuple, Value
            >
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
    typename detail::enable_if_failure<
        result_of::push_front<Tuple, Value>
    >::type push_front(Tuple&& tuple, Value&& value)
    {
        detail::_explain_push_front(
            std::forward<Tuple>(tuple), std::forward<Value>(value));
    }

    template <typename Tuple, typename Value>
    typename detail::enable_if_failure<
        result_of::push_front<Tuple, expand_tuple_t, Value>
    >::type push_front(Tuple&& tuple, expand_tuple_t, Value&& value)
    {
        detail::_explain_push_front(
            std::forward<Tuple>(tuple)
          , expand_tuple_t{}, std::forward<Value>(value));
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_PUSH_FRONT_HPP*/
