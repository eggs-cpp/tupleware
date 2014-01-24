//! \file eggs/tupleware/for_each.hpp
// Eggs.Tupleware
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_TUPLEWARE_FOR_EACH_HPP
#define EGGS_TUPLEWARE_FOR_EACH_HPP

#include <eggs/tupleware/core.hpp>
#include <eggs/tupleware/detail/for_each.hpp>

namespace eggs { namespace tupleware
{
    namespace result_of
    {
        template <typename Tuple, typename UnaryFunction>
        struct for_each
          : detail::_result_of_for_each<
                Tuple, UnaryFunction
            >
        {};

        template <typename Tuple, typename UnaryFunction>
        using for_each_t =
            typename for_each<Tuple, UnaryFunction>::type;
    }

    template <typename Tuple, typename UnaryFunction>
    constexpr result_of::for_each_t<Tuple, UnaryFunction>
    for_each(Tuple&& tuple, UnaryFunction&& f)
    EGGS_TUPLEWARE_RETURN(
        detail::for_each(
            std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
    )

    namespace functional
    {
        struct for_each
        {
            template <typename Tuple, typename UnaryFunction>
            constexpr result_of::for_each_t<Tuple, UnaryFunction>
            operator()(Tuple&& tuple, UnaryFunction&& f) const
            EGGS_TUPLEWARE_RETURN(
                detail::for_each(
                    std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f))
            )
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    //! \cond DETAIL
    template <typename Tuple, typename UnaryFunction>
    typename detail::enable_if_failure<
        result_of::for_each<Tuple, UnaryFunction>
    >::type for_each(Tuple&& tuple, UnaryFunction&& f)
    {
        detail::_explain_for_each(
            std::forward<Tuple>(tuple), std::forward<UnaryFunction>(f));
    }
    //! \endcond
}}

#endif /*EGGS_TUPLEWARE_FOR_EACH_HPP*/
