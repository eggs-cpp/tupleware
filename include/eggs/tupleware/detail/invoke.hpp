//! \file eggs/tupleware/detail/invoke.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_DETAIL_INVOKE_HPP
#define EGGS_TUPLEWARE_DETAIL_INVOKE_HPP

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
            typename T, typename Args
          , typename Enable = void
        >
        struct invoke
          : identity<T>
        {};

        template <typename Function, typename ...Args>
        struct invoke<function<Function>, pack<Args...>>
          : function<Function>::template apply<Args...>
        {};

        template <typename ...Args>
        struct invoke<placeholder<0>, pack<Args...>>;

        template <std::size_t I, typename ...Args>
        struct invoke<placeholder<I>, pack<Args...>>
          : at<I - 1, std::tuple<Args...>>
        {};

        template <
            template <typename...> class Template, typename ...T
          , typename ...Args
        >
        struct invoke<
            Template<T...>, pack<Args...>
          , typename std::enable_if<
                is_placeholder_expression<Template<T...>>::value
            >::type
        > : Template<typename invoke<T, pack<Args...>>::type...>
        {};

        template <
            template <typename...> class Template, typename ...T
          , typename ...Args
        >
        struct invoke<
            Template<T...>, pack<Args...>
          , typename std::enable_if<
                !is_placeholder_expression<Template<T...>>::value
            >::type
        > : identity<Template<T...>>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename ...Args>
    void invoke_overload(F const&, Args const&...);

    template <typename F, typename Tuple>
    expand_tuple_t invoke_overload(F const&, expand_tuple_t, Tuple const&);

    ///////////////////////////////////////////////////////////////////////////
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
        std::size_t ...Is
      , typename F, typename Args
    >
    constexpr auto invoke_expand_impl(
        index_sequence<Is...>
      , F&& f, Args&& args)
    EGGS_TUPLEWARE_AUTO_RETURN(
        invoke_impl(
            std::forward<F>(f)
          , at(meta::size_t<Is>{}, std::forward<Args>(args))...)
    )

    ///////////////////////////////////////////////////////////////////////////
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

    template <typename F, typename Args>
    constexpr auto invoke(F&& f, expand_tuple_t, Args&& args)
    EGGS_TUPLEWARE_AUTO_RETURN(
        invoke_expand_impl(
            index_sequence_for_tuple<Args>{}
          , std::forward<F>(f), std::forward<Args>(args))
    )

    ///////////////////////////////////////////////////////////////////////////
    EGGS_TUPLEWARE_RESULT_OF(
        _result_of_invoke
      , ::eggs::tupleware::detail::invoke
    );

    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename ...Args>
    void _explain_invoke(F&& /*f*/, Args&&... /*args*/)
    {
        static_assert(
            inject_context<
                _result_of_invoke<F, Args...>
            >::value
          , "ill-formed invoke expression");
    }
}}}
//! \endcond

#endif /*EGGS_TUPLEWARE_DETAIL_INVOKE_HPP*/
