#include <boost/test/unit_test.hpp>

#include <Durin/Type/BasicTypes.hpp>

using namespace boost::unit_test;

void type_size()
{
    using namespace Durin;
    BOOST_CHECK( 1 == sizeof( kbyte ) );

    BOOST_CHECK( 1 == sizeof( kint8 ) );
    BOOST_CHECK( 2 == sizeof( kint16 ) );
    BOOST_CHECK( 4 == sizeof( kint32 ) );
    BOOST_CHECK( 8 == sizeof( kint64 ) );

    BOOST_CHECK( 1 == sizeof( kint8 ) );
    BOOST_CHECK( 2 == sizeof( kuint16 ) );
    BOOST_CHECK( 4 == sizeof( kuint32 ) );
    BOOST_CHECK( 8 == sizeof( kuint64 ) );
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite* suite = BOOST_TEST_SUITE( "durin" );
    suite->add( BOOST_TEST_CASE( &type_size ) );

    framework::master_test_suite().add( suite );
    return nullptr;
}
