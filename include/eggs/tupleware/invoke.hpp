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
            template <
                typename T, typename Args, typename Enable = void
            >
            struct invoke_impl
              : identity<T>
            {};

            template <typename Function, typename ...Args>
            struct invoke_impl<function<Function>, pack<Args...>>
              : function<Function>::template apply<Args...>
            {};

            template <typename ...Args>
            struct invoke_impl<detail::placeholder<0>, pack<Args...>>;

            template <std::size_t I, typename ...Args>
            struct invoke_impl<detail::placeholder<I>, pack<Args...>>
              : meta::at<I - 1, std::tuple<Args...>>
            {};

            template <
                template <typename...> class Template, typename ...T
              , typename ...Args
            >
            struct invoke_impl<
                Template<T...>, pack<Args...>
              , typename std::enable_if<
                    is_placeholder_expression<Template<T...>>::value
                >::type
            > : Template<typename invoke_impl<T, pack<Args...>>::type...>
            {};

            template <
                template <typename...> class Template, typename ...T
              , typename ...Args
            >
            struct invoke_impl<
                Template<T...>, pack<Args...>
              , typename std::enable_if<
                    !is_placeholder_expression<Template<T...>>::value
                >::type
            > : identity<Template<T...>>
            {};
        }
        //! \endcond

        //! \brief
        //!
        //! \see \link invoke(F&&, Args&&...) \endlink,
        //!      \link invoke(F&&, expand_tuple_t, Args&&) \endlink,
        //!      \link result_of::invoke \endlink,
        //!      \link result_of::invoke_t \endlink,
        //!      \link functional::invoke \endlink
        template <typename Expression, typename ...Args>
        struct invoke
          : detail::invoke_impl<Expression, pack<Args...>>
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
        //! \brief Result type of an invocation of \ref invoke(F&&, Args&&...)
        //!
        //! \see \link meta::invoke \endlink,
        //!      \link invoke(F&&, Args&&...) \endlink,
        //!      \link invoke(F&&, expand_tuple_t, Args&&) \endlink,
        //!      \link result_of::invoke_t \endlink,
        //!      \link functional::invoke \endlink
        template <typename F, typename ...Args>
        struct invoke
          : detail::_result_of_invoke<pack<
                F, Args...
            >>
        {};

        //! \brief Result type of an invocation of
        //! \ref invoke(F&&, expand_tuple_t, Args&&)
        //!
        //! \see \link meta::invoke \endlink,
        //!      \link invoke(F&&, Args&&...) \endlink,
        //!      \link invoke(F&&, expand_tuple_t, Args&&) \endlink,
        //!      \link result_of::invoke_t \endlink,
        //!      \link functional::invoke \endlink
        template <typename F, typename Args>
        struct invoke<F, expand_tuple_t, Args>
          : detail::_result_of_invoke<pack<
                F, expand_tuple_t, Args
            >>
        {};

        //! \brief Alias for \link result_of::invoke \endlink
        //!
        //! \see \link meta::invoke \endlink,
        //!      \link invoke(F&&, Args&&...) \endlink,
        //!      \link invoke(F&&, expand_tuple_t, Args&&) \endlink,
        //!      \link result_of::invoke \endlink,
        //!      \link functional::invoke \endlink
        template <typename F, typename ...Args>
        using invoke_t =
            typename invoke<F, Args...>::type;
    }

    //! \brief Invokes a callable with the arguments given
    //!
    //! \details
    //!
    //! - `(arg1.*f)(arg2, ..., argN)` when `f` is a pointer to a member
    //!   function of a class `T` and `arg1` is an object of type `T` or a
    //!   reference to an object of type `T` or a reference to an object of a
    //!   type derived from `T`;
    //!
    //! - `((*arg1).*f)(arg2, ..., argN)` when `f` is a pointer to a member
    //!   function of a class `T` and `arg1` is not one of the types described
    //!   in the previous item;
    //!
    //! - `arg1.*f` when `sizeof...(Args) == 1` and `f` is a pointer to member
    //!   data of a class `T` and `arg1` is an object of type `T` or a
    //!   reference to an object of type `T` or a reference to an object of a
    //!   type derived from `T`;
    //!
    //! - `(*arg1).*f` when `sizeof...(Args) == 1` and `f` is a pointer to
    //!   member data of a class `T` and `arg1` is not one of the types
    //!   described in the previous item;
    //!
    //! - `f(arg1, arg2, ..., argN)` in all other cases.
    //!
    //! \param f A target callable object.
    //!
    //! \param ...args The arguments to the callable object.
    //!
    //! \requires `INVOKE(std::forward<F>(f), std::forward<Args>(args)...)`
    //! shall be well-formed.
    //!
    //! \returns The result of invoking the callable.
    //!
    //! \throws Nothing unless the invoked callable throws an exception.
    //!
    //! \see \link meta::invoke \endlink,
    //!      \link invoke(F&&, expand_tuple_t, Args&&) \endlink,
    //!      \link result_of::invoke \endlink,
    //!      \link result_of::invoke_t \endlink,
    //!      \link functional::invoke \endlink
    template <typename F, typename ...Args>
    constexpr result_of::invoke_t<F, Args...>
    invoke(F&& f, Args&&... args)
    EGGS_TUPLEWARE_RETURN(
        detail::invoke(std::forward<F>(f), std::forward<Args>(args)...)
    )

    //! \brief Invokes a callable with arguments from a `tuple`
    //!
    //! \param f A target callable object.
    //!
    //! \param args The arguments to the callable object.
    //!
    //! \requires `INVOKE(std::forward<F>(f), std::get<Is>(std::forward<Args>(args)...))`
    //! shall be well-formed, where `Is` is a variadic pack of `std::size_t`
    //! values indexing the tuple `args`.
    //!
    //! \returns The result of invoking the callable.
    //!
    //! \throws Nothing unless the invoked callable throws an exception.
    //!
    //! \see \link meta::invoke \endlink,
    //!      \link invoke(F&&, Args&&...) \endlink,
    //!      \link result_of::invoke \endlink,
    //!      \link result_of::invoke_t \endlink,
    //!      \link functional::invoke \endlink
    template <typename F, typename Args>
    constexpr result_of::invoke_t<F, expand_tuple_t, Args>
    invoke(F&& f, expand_tuple_t, Args&& args)
    EGGS_TUPLEWARE_RETURN(
        detail::invoke(
            std::forward<F>(f)
          , expand_tuple_t{}, std::forward<Args>(args))
    )

    namespace functional
    {
        //! \brief Functional version of \ref invoke(F&&, Args&&...),
        //! \ref invoke(F&&, expand_tuple_t, Args&&)
        //!
        //! \see \link meta::invoke \endlink,
        //!      \link invoke(F&&, Args&&...) \endlink,
        //!      \link invoke(F&&, expand_tuple_t, Args&&) \endlink,
        //!      \link result_of::invoke \endlink,
        //!      \link result_of::invoke_t \endlink
        struct invoke
        {
            //! \copydoc invoke(F&&, Args&&...)
            //!
            //! \remarks This `operator()` shall not participate in overload
            //! resolution if the invoke expression is ill-formed.
            template <typename F, typename ...Args>
            constexpr result_of::invoke_t<F, Args...>
            operator()(F&& f, Args&&... args) const
            EGGS_TUPLEWARE_RETURN(
                detail::invoke(std::forward<F>(f), std::forward<Args>(args)...)
            )

            //! \copydoc invoke(F&&, expand_tuple_t, Args&&)
            //!
            //! \remarks This `operator()` shall not participate in overload
            //! resolution if the invoke expression is ill-formed.
            template <typename F, typename Args>
            constexpr result_of::invoke_t<F, expand_tuple_t, Args>
            operator()(F&& f, expand_tuple_t, Args&& args) const
            EGGS_TUPLEWARE_RETURN(
                detail::invoke(
                    std::forward<F>(f)
                  , expand_tuple_t{}, std::forward<Args>(args))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename F, typename ...Args>
    typename tupleware::detail::enable_if_failure<
        result_of::invoke<F, Args...>
    >::type invoke(F&& f, Args&&... args)
    {
        static_assert(
            tupleware::detail::inject_context<
                result_of::invoke<F, Args...>
            >::value
          , "ill-formed invoke expression");
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_INVOKE_HPP*/
