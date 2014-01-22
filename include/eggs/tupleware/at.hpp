//! \file eggs/tupleware/at.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_AT_HPP
#define EGGS_TUPLEWARE_AT_HPP

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
                std::size_t I, typename Tuple
              , typename Enable = void
            >
            struct at_impl;

            template <std::size_t I, typename Tuple>
            struct at_impl<I, Tuple const>;

            template <std::size_t I, typename Tuple>
            struct at_impl<
                I, Tuple
              , typename std::enable_if<
                    (I < std::tuple_size<Tuple>::value)
                >::type
            > : identity<typename std::tuple_element<I, Tuple>::type>
            {};
        }
        //! \endcond

        //! \brief The `I`th element of `Tuple`
        //!
        //! \details Has a BaseCharacteristic of `meta::identity<TI>`, where
        //! `TI` is the type of the `I`th element of `Tuple` and indexing is
        //! zero-based.
        //!
        //! \tparam I The index of the element to be queried.
        //!
        //! \tparam Tuple The type which is queried for its `I`th element.
        //!
        //! \requires The type `Tuple` shall support the \ref tuple_protocol.
        //! The index `I` shall be within the bounds of `Tuple`.
        //!
        //! \see \link at<I>(Tuple&&) \endlink,
        //!      \link result_of::at \endlink,
        //!      \link result_of::at_t \endlink,
        //!      \link functional::at \endlink
        template <std::size_t I, typename Tuple>
        struct at
          : detail::at_impl<I, Tuple>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    namespace detail
    {
        template <
            std::size_t I
          , typename Tuple
          , typename Enable =
                typename std::enable_if<
                    (I < result_of::size_t<Tuple>::value)
                >::type
        >
        constexpr auto at(meta::size_t<I>, Tuple&& tuple)
        EGGS_TUPLEWARE_AUTO_RETURN(
            std::get<I>(std::forward<Tuple>(tuple))
        )

        ///////////////////////////////////////////////////////////////////////
        EGGS_TUPLEWARE_RESULT_OF(
            _result_of_at
          , ::eggs::tupleware::detail::at
        );
    }
    //! \endcond

    namespace result_of
    {
        //! \brief Result type of an invocation of \ref at<I>(Tuple&&)
        //!
        //! \remarks The nested typedef `type` is equivalent to
        //! `decltype(at<I>(std::declval<Tuple>()))` when such expression is
        //! well-formed, otherwise it does not exist.
        //!
        //! \see \link meta::at \endlink,
        //!      \link at<I>(Tuple&&) \endlink,
        //!      \link result_of::at_t \endlink,
        //!      \link functional::at \endlink
        template <std::size_t I, typename Tuple>
        struct at
          : detail::_result_of_at<pack<
                meta::size_t<I>
              , Tuple
            >>
        {};

        //! \brief Alias for \link result_of::at \endlink
        //!
        //! \see \link meta::at \endlink,
        //!      \link at<I>(Tuple&&) \endlink,
        //!      \link result_of::at \endlink,
        //!      \link functional::at \endlink
        template <std::size_t I, typename Tuple>
        using at_t =
            typename at<I, Tuple>::type;
    }

    //! \brief The `I`th element of `tuple`
    //!
    //! \details Retrieves the `I`th element of `tuple`, where indexing is
    //! zero-based.
    //!
    //! \tparam I The index of the element to be retrieved.
    //!
    //! \param tuple A tuple object which is queried for its `I`th element.
    //!
    //! \requires The type of `tuple` shall support the \ref tuple_protocol.
    //! The index `I` shall be within the bounds of `tuple`.
    //!
    //! \returns A reference to the `I`th element of `tuple`. If `tuple` is an
    //! lvalue, the returned reference is an lvalue reference. If `tuple` is
    //! an rvalue, the returned reference is an rvalue reference unless the
    //! element to be retrieved is an lvalue reference.
    //!
    //! \note Forwarding an element is thus effectively achieved with
    //! `at<I>(std::move(tuple))`. This is functionally equivalent to
    //! `std::forward<IT>(at<I>(tuple))`, where `IT` is the type of the `I`th
    //! element of `Tuple` and indexing is zero-based.
    //!
    //! \see \link meta::at \endlink,
    //!      \link result_of::at \endlink,
    //!      \link result_of::at_t \endlink,
    //!      \link functional::at \endlink
    template <std::size_t I, typename Tuple>
    constexpr result_of::at_t<I, Tuple>
    at(Tuple&& tuple)
    EGGS_TUPLEWARE_RETURN(
        detail::at(meta::size_t<I>{}, std::forward<Tuple>(tuple))
    )

    namespace functional
    {
        //! \brief Functional version of \ref at<I>(Tuple&&)
        //!
        //! \see \link meta::at \endlink,
        //!      \link at<I>(Tuple&&) \endlink,
        //!      \link result_of::at \endlink,
        //!      \link result_of::at_t \endlink
        template <std::size_t I>
        struct at
        {
            //! \copydoc at<I>(Tuple&)
            //!
            //! \remarks This `operator()` shall not participate in overload
            //! resolution if the invoke expression is ill-formed.
            template <typename Tuple>
            constexpr result_of::at_t<I, Tuple>
            operator()(Tuple&& tuple) const
            EGGS_TUPLEWARE_RETURN(
                detail::at(meta::size_t<I>{}, std::forward<Tuple>(tuple))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t I, typename Tuple>
    typename tupleware::detail::enable_if_failure<
        result_of::at<I, Tuple>
    >::type at(Tuple&& /*tuple*/)
    {
        static_assert(
            result_of::is_tuple_t<Tuple>::value
          , "'tuple' argument is not a Tuple");

        static_assert(
            I < result_of::size_t<Tuple>::value
          , "'I' argument is out of bounds");
    }
}}

#endif /*EGGS_TUPLEWARE_AT_HPP*/
