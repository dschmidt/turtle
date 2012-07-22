// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_FUNCTION_HPP_INCLUDED
#define MOCK_FUNCTION_HPP_INCLUDED

#include "../config.hpp"
#include "../error.hpp"
#include "../log.hpp"
#include "../constraints.hpp"
#include "../sequence.hpp"
#include "check.hpp"
#include "action.hpp"
#include "verifiable.hpp"
#include "type_name.hpp"
#include "context.hpp"
#include "invocation.hpp"
#include "expectation_base.hpp"
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/optional.hpp>
#include <ostream>
#include <vector>
#include <list>

#define MOCK_NUM_ARGS 0
#define MOCK_NUM_ARGS_0
#include "function_template.hpp"
#undef MOCK_NUM_ARGS_0
#undef MOCK_NUM_ARGS

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(1,MOCK_MAX_ARGS,"function_iterate.hpp"))
#include BOOST_PP_ITERATE()
#undef BOOST_PP_ITERATION_PARAMS_1

namespace mock
{
    template< typename Signature >
    bool verify( const function< Signature >& f )
    {
        return f.verify();
    }
    template< typename Signature >
    void reset( function< Signature >& f )
    {
        f.reset();
    }
} // mock

#endif // MOCK_FUNCTION_HPP_INCLUDED