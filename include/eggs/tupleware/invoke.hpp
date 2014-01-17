//! \file eggs/tupleware/invoke.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_INVOKE_HPP
#define EGGS_TUPLEWARE_INVOKE_HPP

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
            template <typename T, typename ...Args>
            struct invoke_impl
            {
                using type = T;
            };

            template <typename ...Args>
            struct invoke_impl<detail::placeholder<0>, Args...>
            {};

            template <std::size_t I, typename ...Args>
            struct invoke_impl<detail::placeholder<I>, Args...>
              : meta::at<I - 1, std::tuple<Args...>>
            {};

            template <
                template <typename...> class Template, typename ...T
              , typename ...Args
            >
            struct invoke_impl<Template<T...>, Args...>
            {
                using type =
                    Template<typename invoke_impl<T, Args>::type...>;
            };
        }
        //! \endcond

        template <typename MetaFunction, typename ...Args>
        struct invoke
          : detail::invoke_impl<MetaFunction, Args...>::type
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
        template <typename F, typename ...Args>
        void invoke_overload(F const&, Args const&...);

        template <typename F, typename Tuple>
        expand_tuple_t invoke_overload(F const&, expand_tuple_t, Tuple const&);

        ///////////////////////////////////////////////////////////////////////
        //! (t1.*f)(t2, ..., tN) when f is a pointer to a member function of a
        //! class T and t1 is an object of type T or a reference to an object
        //! of type T or a reference to an object of a type derived from T;
        template <typename F, typename Arg0, typename ...Args>
        constexpr auto invoke_impl(F&& f, Arg0&& arg0, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            (std::forward<Arg0>(arg0).*std::forward<F>(f))(
                std::forward<Args>(args)...)
        )

        //! ((*t1).*f)(t2, ..., tN) when f is a pointer to a member function of
        //! a class T and t1 is not one of the types described in the previous
        //! item;
        template <typename F, typename Arg0, typename ...Args>
        constexpr auto invoke_impl(F&& f, Arg0&& arg0, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            ((*std::forward<Arg0>(arg0)).*std::forward<F>(f))(
                std::forward<Args>(args)...)
        )

        //! t1.*f when N == 1 and f is a pointer to member data of a class T
        //! and t1 is an object of type T or a reference to an object of type T
        //! or a reference to an object of a type derived from T;
        template <typename F, typename Arg0>
        constexpr auto invoke_impl(F&& f, Arg0&& arg0)
        EGGS_TUPLEWARE_AUTO_RETURN(
            std::forward<Arg0>(arg0).*std::forward<F>(f)
        )

        //! (*t1).*f when N == 1 and f is a pointer to member data of a class T
        //! and t1 is not one of the types described in the previous item;
        template <typename F, typename Arg0>
        constexpr auto invoke_impl(F&& f, Arg0&& arg0)
        EGGS_TUPLEWARE_AUTO_RETURN(
            (*std::forward<Arg0>(arg0)).*std::forward<F>(f)
        )

        //! f(t1, t2, ..., tN) in all other cases.
        template <typename F, typename ...Args>
        constexpr auto invoke_impl(F&& f, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            std::forward<F>(f)(std::forward<Args>(args)...)
        )

        template <
            typename F, typename ...Args
          , typename Enable =
                typename std::enable_if<
                    !std::is_same<
                        decltype(invoke_overload(
                            std::declval<F>(), std::declval<Args>()...))
                      , expand_tuple_t
                    >::value
                >::type
        >
        constexpr auto invoke(F&& f, Args&&... args)
        EGGS_TUPLEWARE_AUTO_RETURN(
            invoke_impl(std::forward<F>(f), std::forward<Args>(args)...)
        )

        ///////////////////////////////////////////////////////////////////////
        template <
            std::size_t ...Is
          , typename F, typename Tuple
        >
        constexpr auto invoke_expand_impl(
            index_sequence<Is...>
          , F&& f, Tuple&& tuple)
        EGGS_TUPLEWARE_AUTO_RETURN(
            invoke_impl(
                std::forward<F>(f)
              , at(meta::size_t<Is>{}, std::forward<Tuple>(tuple))...)
        )

        template <typename F, typename Tuple>
        constexpr auto invoke(F&& f, expand_tuple_t, Tuple&& tuple)
        EGGS_TUPLEWARE_AUTO_RETURN(
            invoke_expand_impl(
                index_sequence_for_tuple<Tuple>{}
              , std::forward<F>(f), std::forward<Tuple>(tuple))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_invoke
          , ::eggs::tupleware::detail::invoke
        );
    }
    //! \endcond

    namespace result_of
    {
        template <typename F, typename ...Args>
        struct invoke
          : detail::_result_of_invoke<pack<
                F, Args...
            >>
        {};

        template <typename F, typename Tuple>
        struct invoke<F, expand_tuple_t, Tuple>
          : detail::_result_of_invoke<pack<
                F, expand_tuple_t, Tuple
            >>
        {};

        template <typename F, typename ...Args>
        using invoke_t =
            typename invoke<F, Args...>::type;
    }

    template <typename F, typename ...Args>
    constexpr result_of::invoke_t<F, Args...>
    invoke(F&& f, Args&&... args)
    EGGS_TUPLEWARE_RETURN(
        detail::invoke(std::forward<F>(f), std::forward<Args>(args)...)
    )

    template <typename F, typename Tuple>
    constexpr result_of::invoke_t<F, expand_tuple_t, Tuple>
    invoke(F&& f, expand_tuple_t, Tuple&& tuple)
    EGGS_TUPLEWARE_RETURN(
        detail::invoke(
            std::forward<F>(f)
          , expand_tuple_t{}, std::forward<Tuple>(tuple))
    )

    namespace functional
    {
        struct invoke
        {
            template <typename F, typename ...Args>
            constexpr result_of::invoke_t<F, Args...>
            operator()(F&& f, Args&&... args) const
            EGGS_TUPLEWARE_RETURN(
                detail::invoke(std::forward<F>(f), std::forward<Args>(args)...)
            )

            template <typename F, typename Tuple>
            constexpr result_of::invoke_t<F, expand_tuple_t, Tuple>
            operator()(F&& f, expand_tuple_t, Tuple&& tuple) const
            EGGS_TUPLEWARE_RETURN(
                detail::invoke(
                    std::forward<F>(f)
                  , expand_tuple_t{}, std::forward<Tuple>(tuple))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename ...Args>
    typename tupleware::detail::enable_if_failure<
        result_of::invoke<F, Args...>
    >::type invoke(F&& f, Args&&... args)
    {
        //std::forward<F>(f)(std::forward<Args>(args)...);

        static_assert(
            tupleware::detail::inject_context<
                result_of::invoke<F, Args...>
            >::value
          , "ill-formed invoke expression");
    }
}}

#endif /*EGGS_TUPLEWARE_INVOKE_HPP*/
