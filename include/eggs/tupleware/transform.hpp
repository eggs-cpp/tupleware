//! \file eggs/tupleware/transform.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_TRANSFORM_HPP
#define EGGS_TUPLEWARE_TRANSFORM_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/transform.hpp>

namespace eggs { namespace tupleware
{
    namespace meta
    {
        template <typename Tuple, typename UnaryMetaFunction>
        struct transform
          : detail::meta::transform<Tuple, UnaryMetaFunction>
        {};
    }

    ///////////////////////////////////////////////////////////////////////////
    namespace result_of
    {
        template <typename Tuple, typename UnaryFunction>
        struct transform
          : detail::_result_of_transform<
                Tuple, UnaryFunction
            >
        {};

        template <typename Tuple, typename UnaryFunction>
        using transform_t =
            typename transform<Tuple, UnaryFunction>::type;
    }

    template <typename Tuple, typename UnaryFunction>
    constexpr result_of::transform_t<Tuple, UnaryFunction>
    transform(Tuple&& tuple, UnaryFunction&& f)
    EGGS_TUPLEWARE_RETURN(
        detail::transform(
            std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
    )

    namespace functional
    {
        struct transform
        {
            template <typename Tuple, typename UnaryFunction>
            constexpr result_of::transform_t<Tuple, UnaryFunction>
            operator()(Tuple&& tuple, UnaryFunction&& f) const
            EGGS_TUPLEWARE_RETURN(
                detail::transform(
                    std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple, typename UnaryFunction>
    typename detail::enable_if_failure<
        result_of::transform<Tuple, UnaryFunction>
    >::type transform(Tuple&& tuple, UnaryFunction&& f)
    {
        detail::_explain_transform(
            std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f));
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_TRANSFORM_HPP*/
