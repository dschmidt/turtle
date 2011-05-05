
//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "silent_error.hpp"

#define MOCK_ERROR_POLICY silent_error
#include <turtle/mock.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#define TEXT(z, n, text) text

namespace
{
    struct my_custom_mock
    {
        MOCK_METHOD_EXT( method, MOCK_MAX_ARGS, void( BOOST_PP_ENUM(MOCK_MAX_ARGS, TEXT, int) ), method )
        MOCK_METHOD_EXT( method2, MOCK_MAX_ARGS, int( BOOST_PP_ENUM(MOCK_MAX_ARGS, TEXT, int) ), method2 )
    };
}

BOOST_AUTO_TEST_CASE( call_mock_method_with_max_number_of_args )
{
    my_custom_mock m;
    MOCK_EXPECT( m, method ).once().with( BOOST_PP_ENUM(MOCK_MAX_ARGS, TEXT, 0) );
    m.method( BOOST_PP_ENUM(MOCK_MAX_ARGS, TEXT, 0) );
}

BOOST_AUTO_TEST_CASE( call_mock_method_with_max_number_of_args_and_returning_something )
{
    my_custom_mock m;
    MOCK_EXPECT( m, method2 ).once().with( BOOST_PP_ENUM(MOCK_MAX_ARGS, TEXT, 0) ).returns( 42 );
    BOOST_CHECK_EQUAL( 42, m.method2( BOOST_PP_ENUM(MOCK_MAX_ARGS, TEXT, 0) ) );
}
