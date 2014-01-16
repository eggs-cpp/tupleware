// Copyright (c) 2014 Agustin K-ballo Berge
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_IS_TUPLE_HPP
#define EGGS_TUPLEWARE_IS_TUPLE_HPP

#include <eggs/tupleware/core.hpp>

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
                typename T
              , typename Enable = void
            >
            struct is_tuple_impl
              : identity<std::false_type>
            {};

            template <typename T>
            struct is_tuple_impl<T const>
              : identity<std::false_type>
            {};

            template <typename T>
            struct is_tuple_impl<
                T
              , typename std::enable_if<
                    (std::tuple_size<T>::value >= 0)
                >::type
            > : identity<std::true_type>
            {};
        }
        //! \endcond

        //! \brief Whether the type `T` supports the \ref tuple_protocol
        //!
        //! Has a BaseCharacteristic of `std::true_type` if the type `T`
        //! supports the \ref tuple_protocol, otherwise it has a
        //! BaseCharacteristic of `std::false_type`.
        //!
        //! \tparam T The type to check for \ref tuple_protocol support.
        //!
        //! \note The specializations required by the \ref tuple_protocol
        //! must be visible at the point of instantiation of this trait.
        //!
        //! \see \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link result_of::is_tuple \endlink,
        //!      \link functional::is_tuple \endlink
        template <typename T>
        struct is_tuple
          : detail::is_tuple_impl<T>::type
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
        template <typename T>
        constexpr auto is_tuple(meta::identity<T>)
        EGGS_TUPLEWARE_AUTO_RETURN(
            typename meta::is_tuple<typename std::decay<T>::type>::type{}
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_is_tuple
          , ::eggs::tupleware::detail::is_tuple
        );
    }
    //! \endcond

    namespace result_of
    {
        //! \brief Result type of an invocation of \ref is_tuple(T&&)
        //!
        //! \remarks The nested typedef `type` is equivalent to
        //! `decltype(is_tuple(std::declval<T>()))` when such expression is
        //! well-formed, otherwise it does not exist.
        //!
        //! \note The specializations required by the \ref tuple_protocol
        //! must be visible at the point of instantiation of this trait.
        //!
        //! \see \link meta::is_tuple \endlink,
        //!      \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link functional::is_tuple \endlink
        template <typename T>
        struct is_tuple
          : detail::_result_of_is_tuple<pack<
                meta::identity<T>
            >>
        {};

        //! \brief Alias for \link result_of::is_tuple \endlink
        template <typename T>
        using is_tuple_t =
            typename is_tuple<T>::type;
    }

    //! \brief Whether the type `T` supports the \ref tuple_protocol
    //!
    //! \tparam T A type that is checked for \ref tuple_protocol support.
    //!
    //! \returns An instance of `std::true_type` if the type `T`
    //! supports the \ref tuple_protocol, an instance of `std::false_type`
    //! otherwise.
    //!
    //! \remarks Equivalent to
    //! `typename meta::is_tuple<typename std::decay<T>::type>::type{}`
    //!
    //! \note The specializations required by the \ref tuple_protocol
    //! must be visible at the point of instantiation of this trait.
    //!
    //! \see \link meta::is_tuple \endlink,
    //!      \link is_tuple(T&&) \endlink,
    //!      \link result_of::is_tuple \endlink,
    //!      \link functional::is_tuple \endlink
    template <typename T>
    constexpr result_of::is_tuple_t<T>
    is_tuple()
    EGGS_TUPLEWARE_RETURN(
        detail::is_tuple(meta::identity<T>{})
    )

    //! \brief Whether the type of `value` supports the \ref tuple_protocol
    //!
    //! \param value An object whose type is checked for \ref tuple_protocol
    //! support.
    //!
    //! \returns An instance of `std::true_type` if the type `T`
    //! supports the \ref tuple_protocol, an instance of `std::false_type`
    //! otherwise.
    //!
    //! \remarks Equivalent to
    //! `typename meta::is_tuple<typename std::decay<decltype(value)>::type>::type{}`
    //!
    //! \note The specializations required by the \ref tuple_protocol
    //! must be visible at the point of instantiation of this trait.
    //!
    //! \see \link meta::is_tuple \endlink,
    //!      \link is_tuple() \endlink,
    //!      \link result_of::is_tuple \endlink,
    //!      \link functional::is_tuple \endlink
    template <typename T>
    constexpr result_of::is_tuple_t<T>
    is_tuple(T&& value)
    EGGS_TUPLEWARE_RETURN(
        detail::is_tuple(
            (static_cast<void>(value), meta::identity<T>{}))
    )

    namespace functional
    {
        //! \brief Functional version of \ref is_tuple(T&&)
        //!
        //! \note The specializations required by the \ref tuple_protocol
        //! must be visible at the point of instantiation of this trait.
        //!
        //! \see \link meta::is_tuple \endlink,
        //!      \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link result_of::is_tuple \endlink
        struct is_tuple
        {
            //! \copydoc is_tuple(T&&)
            //!
            //! \remarks This `operator()` will not participate in overload
            //! resolution if the invoke expression is ill-formed.
            template <typename T>
            constexpr auto operator()(T&& value) const
            EGGS_TUPLEWARE_AUTO_RETURN(
                detail::is_tuple(
                    static_cast<void>(value), meta::identity<T>{})
            )
        };
    }
}}

#endif /*EGGS_TUPLEWARE_IS_TUPLE_HPP*/
