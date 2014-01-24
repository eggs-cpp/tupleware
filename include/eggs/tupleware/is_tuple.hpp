//! \file eggs/tupleware/is_tuple.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_IS_TUPLE_HPP
#define EGGS_TUPLEWARE_IS_TUPLE_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/is_tuple.hpp>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        //! \brief Whether the type `T` supports the \ref tuple_protocol
        //!
        //! \details Has a BaseCharacteristic of `std::true_type` if the type
        //! `T` supports the \ref tuple_protocol, otherwise it has a
        //! BaseCharacteristic of `std::false_type`.
        //!
        //! \tparam T A type that is checked for \ref tuple_protocol support.
        //!
        //! \see \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link result_of::is_tuple \endlink,
        //!      \link result_of::is_tuple_t \endlink,
        //!      \link functional::is_tuple \endlink
        template <typename T>
        struct is_tuple
          : detail::meta::is_tuple<T>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        //! \brief Result type of an invocation of \ref is_tuple(T&&)
        //!
        //! \remarks The nested typedef `type` is equivalent to
        //! `decltype(is_tuple(std::declval<T>()))` when such expression is
        //! well-formed, otherwise it does not exist.
        //!
        //! \see \link meta::is_tuple \endlink,
        //!      \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link result_of::is_tuple_t \endlink,
        //!      \link functional::is_tuple \endlink
        template <typename T>
        struct is_tuple
          : detail::_result_of_is_tuple<
                meta::identity<T>
            >
        {};

        //! \brief Alias for \link result_of::is_tuple \endlink
        //!
        //! \see \link meta::is_tuple \endlink,
        //!      \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link result_of::is_tuple \endlink,
        //!      \link functional::is_tuple \endlink
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
    //! \see \link meta::is_tuple \endlink,
    //!      \link is_tuple(T&&) \endlink,
    //!      \link result_of::is_tuple \endlink,
    //!      \link result_of::is_tuple_t \endlink,
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
    //! \see \link meta::is_tuple \endlink,
    //!      \link is_tuple() \endlink,
    //!      \link result_of::is_tuple \endlink,
    //!      \link result_of::is_tuple_t \endlink,
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
        //! \see \link meta::is_tuple \endlink,
        //!      \link is_tuple() \endlink,
        //!      \link is_tuple(T&&) \endlink,
        //!      \link result_of::is_tuple \endlink,
        //!      \link result_of::is_tuple_t \endlink
        struct is_tuple
        {
            //! \copydoc is_tuple(T&&)
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
