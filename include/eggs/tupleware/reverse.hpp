//! \file eggs/tupleware/reverse.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_REVERSE_HPP
#define EGGS_TUPLEWARE_REVERSE_HPP

#include <eggs/tupleware/at.hpp>
#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/is_tuple.hpp>
#include <eggs/tupleware/size.hpp>

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
              , typename Tuple
              , typename Enable = void
            >
            struct reverse_impl
            {};

            template <
                std::size_t ...Is
              , typename Tuple
            >
            struct reverse_impl<
                tupleware::detail::index_sequence<Is...>
              , Tuple
              , typename std::enable_if<
                    is_tuple<Tuple>::value
                >::type
            >
            {
                using type =
                    tupleware::tuple<
                        typename at<size<Tuple>::value - Is - 1, Tuple>::type...
                    >;
            };
        }
        //! \endcond

        template <typename Tuple>
        struct reverse
          : detail::reverse_impl<
                tupleware::detail::index_sequence_for_tuple<Tuple>
              , Tuple
            >
        {
        private:
            Tuple _base;
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
        template <
            std::size_t ...Is
          , typename Tuple
        >
        constexpr auto reverse_impl(
            index_sequence<Is...>
          , Tuple&& tuple)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::reverse<
                typename std::decay<Tuple>::type
            >::type{
                at(
                   meta::size_t<result_of::size_t<Tuple>::value - Is - 1>{}
                 , std::forward<Tuple>(tuple))...}
        )

        template <typename Tuple>
        constexpr auto reverse(Tuple&& tuple)
        EGGS_TUPLEWARE_AUTO_RETURN(
            reverse_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<Tuple>(tuple))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_reverse
          , ::eggs::tupleware::detail::reverse
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename Tuple>
        struct reverse
          : detail::_result_of_reverse<pack<
                Tuple
            >>
        {};

        template <typename Tuple>
        using reverse_t =
            typename reverse<Tuple>::type;
    }

    template <typename Tuple>
    constexpr result_of::reverse_t<Tuple>
    reverse(Tuple&& tuple)
    EGGS_TUPLEWARE_RETURN(
        detail::reverse(std::forward<Tuple>(tuple))
    )

    namespace functional
    {
        struct reverse
        {
            template <typename Tuple>
            constexpr result_of::reverse_t<Tuple>
            operator()(Tuple&& tuple) const
            EGGS_TUPLEWARE_RETURN(
                detail::reverse(std::forward<Tuple>(tuple))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Tuple>
    typename tupleware::detail::enable_if_failure<
        result_of::reverse<Tuple>
    >::type reverse(Tuple&& /*tuple*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");
    }
}}

namespace std
{
    template <typename Tuple>
    class tuple_size<::eggs::tupleware::meta::reverse<Tuple>>
      : public tuple_size<Tuple>
    {};

    template <std::size_t I, typename Tuple>
    class tuple_element<I, ::eggs::tupleware::meta::reverse<Tuple>>
      : public tuple_element<I, Tuple>
    {};

    template <std::size_t I, typename Tuple>
    constexpr auto get(
        ::eggs::tupleware::meta::reverse<Tuple>& tuple)
        noexcept(noexcept(std::get<tuple_size<Tuple>::value - I - 1>(
            tuple.base())))
     -> decltype(std::get<tuple_size<Tuple>::value - I - 1>(
            tuple.base()))
    {
        return std::get<tuple_size<Tuple>::value - I - 1>(
            tuple.base());
    }

    template <std::size_t I, typename Tuple>
    constexpr auto get(
        ::eggs::tupleware::meta::reverse<Tuple>&& tuple)
        noexcept(noexcept(std::get<tuple_size<Tuple>::value - I - 1>(
            std::move(tuple.base()))))
     -> decltype(std::get<tuple_size<Tuple>::value - I - 1>(
            std::move(tuple.base())))
    {
        return std::get<tuple_size<Tuple>::value - I - 1>(
            std::move(tuple.base()));
    }

    template <std::size_t I, typename Tuple>
    constexpr auto get(
        ::eggs::tupleware::meta::reverse<Tuple> const& tuple)
        noexcept(noexcept(std::get<tuple_size<Tuple>::value - I - 1>(
            tuple.base())))
     -> decltype(std::get<tuple_size<Tuple>::value - I - 1>(
            tuple.base()))
    {
        return std::get<tuple_size<Tuple>::value - I - 1>(
            tuple.base());
    }
}

#endif /*EGGS_TUPLEWARE_REVERSE_HPP*/
