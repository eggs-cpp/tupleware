//! \file eggs/tupleware/size.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_SIZE_HPP
#define EGGS_TUPLEWARE_SIZE_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/size.hpp>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        //! \brief The number of elements in the type `Tuple`
        //!
        //! \details Has a BaseCharacteristic of
        //! `std::integral_constant<std::size_t, Value>` where `Value` is the
        //! number of elements in the type `Tuple`.
        //!
        //! \tparam Tuple A type that is queried for its number of elements.
        //!
        //! \requires The type `Tuple` shall support the \ref tuple_protocol.
        //!
        //! \see \link size() \endlink,
        //!      \link size(Tuple const&) \endlink,
        //!      \link result_of::size \endlink,
        //!      \link result_of::size_t \endlink,
        //!      \link functional::size \endlink
        template <typename Tuple>
        struct size
          : detail::meta::size<Tuple>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        //! \brief Result type of an invocation of \ref size(Tuple const&)
        //!
        //! \remarks The nested typedef `type` is equivalent to
        //! `decltype(size(std::declval<Tuple>()))` when such expression is
        //! well-formed, otherwise it does not exist.
        //!
        //! \see \link meta::size \endlink,
        //!      \link size() \endlink,
        //!      \link size(Tuple const&) \endlink,
        //!      \link result_of::size_t \endlink,
        //!      \link functional::size \endlink
        template <typename Tuple>
        struct size
          : detail::_result_of_size<
                meta::identity<Tuple>
            >
        {};

        //! \brief Alias for \link result_of::size \endlink
        //!
        //! \see \link meta::size \endlink,
        //!      \link size() \endlink,
        //!      \link size(Tuple const&) \endlink,
        //!      \link result_of::size \endlink
        //!      \link functional::size \endlink,
        template <typename Tuple>
        using size_t =
            typename size<Tuple>::type;
    }

    //! \brief The number of elements in the type `Tuple`
    //!
    //! \tparam Tuple A type that is queried for its number of elements.
    //!
    //! \requires The type `Tuple` shall support the \ref tuple_protocol.
    //!
    //! \returns An instance of `std::integral_constant<std::size_t, Value>`
    //! where `Value` is the number of elements in the type `Tuple`.
    //!
    //! \remarks Equivalent to
    //! `typename meta::tuple_size<typename std::decay<T>::type>::type{}`
    //!
    //! \see \link meta::size \endlink,
    //!      \link size(Tuple const&) \endlink,
    //!      \link result_of::size \endlink,
    //!      \link result_of::size_t \endlink,
    //!      \link functional::size \endlink
    template <typename Tuple>
    constexpr result_of::size_t<Tuple>
    size()
    EGGS_TUPLEWARE_RETURN(
        detail::size(meta::identity<Tuple>{})
    )

    //! \brief The number of elements in the type of `tuple`
    //!
    //! \param tuple An object whose type is queried for its number of
    //! elements.
    //!
    //! \requires The type of `tuple` shall support the \ref tuple_protocol.
    //!
    //! \returns An instance of `std::integral_constant<std::size_t, Value>`
    //! where `Value` is the number of elements in the type of `tuple`.
    //!
    //! \remarks Equivalent to
    //! `typename meta::tuple_size<typename std::decay<decltype(value)>::type>::type{}`
    //!
    //! \see \link meta::size \endlink,
    //!      \link size() \endlink,
    //!      \link result_of::size \endlink,
    //!      \link result_of::size_t \endlink,
    //!      \link functional::size \endlink
    template <typename Tuple>
    constexpr result_of::size_t<Tuple>
    size(Tuple&& tuple)
    EGGS_TUPLEWARE_RETURN(
        detail::size(
            (static_cast<void>(tuple), meta::identity<Tuple>{}))
    )

    namespace functional
    {
        //! \brief Functional version of \ref size(Tuple const&)
        //!
        //! \see \link meta::size \endlink,
        //!      \link size() \endlink,
        //!      \link size(Tuple const&) \endlink,
        //!      \link result_of::size \endlink
        //!      \link result_of::size_t \endlink,
        struct size
        {
            //! \copydoc size(Tuple const&)
            //!
            //! \remarks This `operator()` shall not participate in overload
            //! resolution if the invoke expression is ill-formed.
            template <typename Tuple>
            constexpr result_of::size_t<Tuple>
            operator()(Tuple&& tuple) const
            EGGS_TUPLEWARE_RETURN(
                detail::size(std::forward<Tuple>(tuple))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple>
    typename detail::enable_if_failure<
        result_of::size<Tuple>
    >::type size(Tuple const& tuple)
    {
        detail::_explain_size(tuple);
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_SIZE_HPP*/
