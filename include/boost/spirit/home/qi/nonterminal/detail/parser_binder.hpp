/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_PARSER_BINDER_DECEMBER_05_2008_0516_PM)
#define BOOST_SPIRIT_PARSER_BINDER_DECEMBER_05_2008_0516_PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <utility>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find_if.hpp>

namespace boost { namespace spirit { namespace qi { namespace detail
{
    // parser_binder for plain rules
    template <typename Parser, typename Auto>
    struct parser_binder
    {
        parser_binder(Parser const& p)
          : p(p) {};

        template <typename Iterator, typename Skipper, typename Context>
        bool operator()(
            Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper) const
        {
            // If Auto is false, the component's attribute is unused.
            return p.parse(first, last, context, skipper, unused);
        }
        Parser p;
    };

    // parser_binder for auto rules
    template <typename Parser>
    struct parser_binder<Parser, mpl::true_>
    {
        parser_binder(Parser const& p)
          : p(p) {};

        template <typename Iterator, typename Skipper, typename Context>
        bool operator()(
            Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper) const
        {
            // If Auto is true, we pass the rule's attribute on to the component.
            return p.parse(first, last, context, skipper
                , fusion::at_c<0>(context.attributes));
        }
        Parser p;
    };

    template <typename Auto, typename Parser>
    inline parser_binder<Parser, Auto>
    bind_parser(Parser const& p)
    {
        return parser_binder<Parser, Auto>(p);
    }

    template <typename Types, typename Pred, typename Default>
    struct extract_param
    {
        typedef typename mpl::find_if<Types, Pred>::type pos;

        typedef typename
            mpl::eval_if<
                is_same<pos, typename mpl::end<Types>::type>
              , mpl::identity<Default>
              , mpl::deref<pos>
            >::type
        type;
    };
}}}}

#endif