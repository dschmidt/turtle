//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/mock.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

#include <boost/noncopyable.hpp>
#include <boost/ref.hpp>

#ifdef _MSC_VER
#   pragma warning( disable : 4355 4505 )
#endif

namespace
{
    struct my_custom_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
    };
}

BOOST_AUTO_TEST_CASE( custom_mock_object_without_macros_and_without_inheriting_from_object )
{
    my_custom_mock m;
    MOCK_EXPECT( m, my_method ).once();
    m.my_method();
}

namespace
{
    struct my_custom_mock_object : mock::object
    {
        my_custom_mock_object()
            : mock::object( "my_custom_mock_object" )
        {}
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
    };
}

BOOST_AUTO_TEST_CASE( custom_mock_object_without_macros )
{
    my_custom_mock_object m;
    MOCK_EXPECT( m, my_method ).once();
    m.my_method();
}

namespace
{
    MOCK_CLASS( my_mock )
    {
        MOCK_METHOD_EXT( my_method, 1, int( int ), my_method )
    };
}

BOOST_AUTO_TEST_CASE( basic_mock_object_usage )
{
    my_mock m;
    MOCK_EXPECT( m, my_method ).once().returns( 0 );
    BOOST_CHECK_EQUAL( 0, m.my_method( 13 ) );
    mock::verify();
    mock::reset();
    MOCK_EXPECT( m, my_method ).once().with( 42 ).returns( 7 );
    BOOST_CHECK_EQUAL( 7, m.my_method( 42 ) );
    mock::verify();
    mock::reset();
    MOCK_EXPECT( m, my_method ).once().returns( 51 );
    BOOST_CHECK_EQUAL( 51, m.my_method( 27 ) );
}

namespace
{
    class my_ambiguited_interface : boost::noncopyable
    {
    public:
        virtual ~my_ambiguited_interface() {}

        virtual void my_method() = 0;
        virtual void my_method( int ) = 0;
    };

    MOCK_BASE_CLASS( my_ambiguited_mock, my_ambiguited_interface )
    {
        MOCK_METHOD_EXT( my_method, 0, void(), tag1 )
        MOCK_METHOD_EXT( my_method, 1, void( int ), tag2 )
    };
}

BOOST_AUTO_TEST_CASE( mock_object_method_disambiguation )
{
    my_ambiguited_mock mock;
    MOCK_EXPECT( mock, tag1 );
    BOOST_CHECK_NO_THROW( mock.my_method() );
    BOOST_CHECK_THROW( mock.my_method( 12 ), mock::exception );
}

namespace
{
    class my_const_ambiguited_interface : boost::noncopyable
    {
    public:
        virtual ~my_const_ambiguited_interface() {}

        virtual void my_method() = 0;
        virtual void my_method() const = 0;
    };

    MOCK_BASE_CLASS( my_const_ambiguited_mock, my_const_ambiguited_interface )
    {
        MOCK_NON_CONST_METHOD_EXT( my_method, 0, void(), tag1 )
        MOCK_CONST_METHOD_EXT( my_method, 0, void(), tag2 )
    };
}

BOOST_AUTO_TEST_CASE( mock_object_method_const_disambiguation )
{
    my_const_ambiguited_mock mock;
    MOCK_EXPECT( mock, tag1 );
    BOOST_CHECK_NO_THROW( mock.my_method() );
    const my_const_ambiguited_mock const_mock;
    BOOST_CHECK_THROW( const_mock.my_method(), mock::exception );
}

BOOST_AUTO_TEST_CASE( mock_functor_in_function_is_supported )
{
    boost::function< int( float, const std::string& ) > func;
    {
        MOCK_FUNCTOR( int( float, const std::string& ) ) f;
        MOCK_EXPECT( f, _ ).once().with( 3, "op" ).returns( 42 );
        func = f;
    }
    BOOST_CHECK_EQUAL( 42, func( 3, "op" ) );
}

BOOST_AUTO_TEST_CASE( mock_functor_name_can_be_customised )
{
    MOCK_FUNCTOR( int( float, const std::string& ) ) f( "my functor" );
}

namespace
{
    struct functor_fixture
    {
        MOCK_FUNCTOR( int( float, const std::string& ) ) f;
    };
}

BOOST_FIXTURE_TEST_CASE( mock_functor_in_fixture_is_supported, functor_fixture )
{
    MOCK_EXPECT( f, _ ).once().with( 3, "op" ).returns( 42 );
    BOOST_CHECK_EQUAL( 42, f( 3.f, "op" ) );
}

namespace
{
    template< typename T >
    struct my_template_mock
    {
        MOCK_METHOD_EXT( my_method, 0, void(), my_method )
        MOCK_METHOD_EXT_TPL( my_method, 2, void( T, std::string ), my_method_t )
        MOCK_METHOD_EXT_TPL( my_other_method, 0, void(), my_other_method )
    };
}

BOOST_AUTO_TEST_CASE( mocking_a_template_class_method_is_supported )
{
    my_template_mock< int > m;
    MOCK_EXPECT( m, my_method_t ).with( 3, "" );
    m.my_method( 3, "" );
}

namespace
{
    template< typename T >
    struct my_template_base_class
    {
        virtual ~my_template_base_class()
        {}
        virtual void my_method( T ) = 0;
        virtual void my_other_method() = 0;
    };
    template< typename T >
    MOCK_BASE_CLASS( my_template_base_class_mock, my_template_base_class< T > )
    {
#if (defined __CYGWIN__) && (__GNUC__ == 3)
        MOCK_METHOD_EXT_TPL( my_method, 1, void( T ), my_method )
        MOCK_METHOD_EXT_TPL( my_other_method, 0, void(), my_other_method )
#else
        MOCK_METHOD_TPL( my_method, 1 )
        MOCK_METHOD_TPL( my_other_method, 0 )
#endif
    };
}

BOOST_AUTO_TEST_CASE( mocking_a_template_base_class_method_is_supported )
{
    my_template_base_class_mock< int > m;
    MOCK_EXPECT( m, my_method ).once().with( 3 );
    m.my_method( 3 );
}

namespace
{
    class my_observer : boost::noncopyable
    {
    public:
        virtual ~my_observer() {}

        virtual void notify( int value ) = 0;
    };

    class my_manager : boost::noncopyable
    {
    public:
        virtual ~my_manager() {}

        virtual my_observer& get_observer() const = 0;
    };

    class my_subject : boost::noncopyable
    {
    public:
        explicit my_subject( my_manager& f )
            : o_( f.get_observer() )
            , value_( 0 )
        {}
        void increment()
        {
            o_.notify( ++value_ );
        }
    private:
        my_observer& o_;
        int value_;
    };

    MOCK_BASE_CLASS( my_mock_observer, my_observer )
    {
        MOCK_METHOD( notify, 1 )
    };

    MOCK_BASE_CLASS( my_mock_manager, my_manager )
    {
        MOCK_METHOD( get_observer, 0 )
    };

    struct fixture
    {
        fixture()
        {
            manager.tag( "(the only one)" );
        }
        my_mock_manager manager;
        my_mock_observer observer;
    };
}

BOOST_FIXTURE_TEST_CASE( basic_mock_object_collaboration_usage, fixture )
{
    MOCK_EXPECT( manager, get_observer ).returns( boost::ref( observer ) );
    my_subject subject( manager );
    MOCK_EXPECT( observer, notify ).once().with( 1 );
    subject.increment();
    MOCK_EXPECT( observer, notify ).once().with( 2 );
    subject.increment();
    MOCK_EXPECT( observer, notify ).once().with( 3 );
    subject.increment();
}