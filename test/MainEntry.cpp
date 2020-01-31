#include <boost/test/unit_test.hpp>

#include <Durin/Type/BasicTypes.hpp>
#include <Durin/Type/FileDescriptor.hpp>
#include <Durin/Scoped/ScopedFileDescriptor.hpp>
#include <Durin/String/String.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace boost::unit_test;

void UtTypeSize()
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

void UtTypeFileDescriptor()
{
    using namespace Durin;

    BOOST_TEST( FileDescriptorTraits::InvalidValue == -1 );

    BOOST_CHECK( false == FileDescriptorTraits::isValid( -1 ) );

    BOOST_CHECK( FileDescriptorTraits::isValid( 0 ) );
    BOOST_CHECK( FileDescriptorTraits::isValid( 1 ) );

    int fd = ::open( "/dev/null", O_RDONLY );
    BOOST_CHECK( FileDescriptorTraits::close( fd ) );
    BOOST_CHECK( FileDescriptorTraits::InvalidValue == fd );
}

void UtScopedFileDescriptor()
{
    using namespace Durin;

    // normal
    ScopedFileDescriptor fd{ ::open( "/dev/null", O_RDONLY ) };
    BOOST_CHECK( fd.isValid() );
    BOOST_CHECK( fd.get() != FileDescriptorTraits::InvalidValue );
    BOOST_CHECK( fd.close() );
    BOOST_CHECK( false == fd.isValid() );
    BOOST_CHECK( fd.get() == FileDescriptorTraits::InvalidValue );

    // reset
    fd.reset( ::open( "/dev/null", O_RDONLY ) );
    BOOST_CHECK( fd.isValid() );
    fd.reset( FileDescriptorTraits::InvalidValue );
    BOOST_CHECK( false == fd.isValid() );
    BOOST_CHECK( fd.get() == FileDescriptorTraits::InvalidValue );

    // release
    fd.reset( ::open( "/dev/null", O_RDONLY ) );
    BOOST_CHECK( fd.isValid() );
    FileDescriptorTraits::ValueType rawFd = fd.release();
    BOOST_CHECK( false == fd.isValid() );
    BOOST_CHECK( fd.get() == FileDescriptorTraits::InvalidValue );
    BOOST_CHECK( false == fd.close() );
    BOOST_CHECK( 0 == ::close( rawFd ) );

    fd.reset( ::open( "/dev/null", O_RDONLY ) );
    BOOST_CHECK( fd.isValid() );
    ScopedFileDescriptor rhs{ FileDescriptorTraits::InvalidValue };
    BOOST_CHECK( false == rhs.isValid() );
    BOOST_CHECK( rhs.get() == FileDescriptorTraits::InvalidValue );
    fd.swap( rhs );
    BOOST_CHECK( false == fd.isValid() );
    BOOST_CHECK( fd.get() == FileDescriptorTraits::InvalidValue );
    BOOST_CHECK( rhs.isValid() );
    BOOST_CHECK( rhs.get() != FileDescriptorTraits::InvalidValue );
}

void UtStringDecimalCheck()
{
    using namespace Durin;

    BOOST_CHECK( String::isDecimalNumber( "0123456789" ) );
    BOOST_CHECK( !String::isDecimalNumber( "0x123ABCDEF" ) );
    BOOST_CHECK( !String::isDecimalNumber( "0x123abcdef" ) );
    BOOST_CHECK( !String::isDecimalNumber( "0x123abcDEF" ) );
    BOOST_CHECK( !String::isDecimalNumber( "hello" ) );
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite* suite = BOOST_TEST_SUITE( "durin" );
    suite->add( BOOST_TEST_CASE( &UtTypeSize ) );
    suite->add( BOOST_TEST_CASE( &UtTypeFileDescriptor ) );
    suite->add( BOOST_TEST_CASE( &UtScopedFileDescriptor ) );
    suite->add( BOOST_TEST_CASE( &UtStringDecimalCheck ) );

    framework::master_test_suite().add( suite );
    return nullptr;
}
