//! \file eggs/tupleware/insert.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_INSERT_HPP
#define EGGS_TUPLEWARE_INSERT_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/insert.hpp>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        template <
            std::size_t Where
          , typename Tuple, typename Value, typename Expand = void
        >
        struct insert
          : detail::meta::insert<
                Where
              , Tuple, Value, Expand
            >
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        template <
            std::size_t Where
          , typename Tuple, typename Value, typename Expand = void
        >
        struct insert
          : detail::_result_of_insert<
                meta::size_t<Where>
              , Tuple, Value, Expand
            >
        {};

        template <
            std::size_t Where
          , typename Tuple, typename Value
        >
        struct insert<Where, Tuple, Value>
          : detail::_result_of_insert<
                meta::size_t<Where>
              , Tuple, Value
            >
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
                detail::insert<Where>(
                    std::forward<Tuple>(tuple), std::forward<Value>(value))
            )

            template <typename Tuple, typename Value>
            constexpr result_of::insert_t<Where, Tuple, expand_tuple_t, Value>
            operator()(Tuple&& tuple, expand_tuple_t, Value&& value) const
            EGGS_TUPLEWARE_RETURN(
                detail::insert<Where>(
                    std::forward<Tuple>(tuple)
                  , expand_tuple_t{}, std::forward<Value>(value))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <std::size_t Where, typename Tuple, typename Value>
    typename detail::enable_if_failure<
        result_of::insert<Where, Tuple, Value>
    >::type insert(Tuple&& tuple, Value&& value)
    {
        detail::_explain_insert<Where>(
            std::forward<Tuple>(tuple), std::forward<Value>(value));
    }

    template <std::size_t Where, typename Tuple, typename Value>
    typename detail::enable_if_failure<
        result_of::insert<Where, Tuple, expand_tuple_t, Value>
    >::type insert(Tuple&& tuple, expand_tuple_t, Value&& value)
    {
        detail::_explain_insert<Where>(
            std::forward<Tuple>(tuple)
          , expand_tuple_t{}, std::forward<Value>(value));
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_INSERT_HPP*/
