#include <Durin/Config/Config.hpp>
#include <include/Durin/Config/Export.hpp>

#include <Durin/Type/FileDescriptor.hpp>

#include <unistd.h>

namespace Durin {

bool FileDescriptorTraits::close( ValueType& fd )
{
    int rc = ::close( fd );
    fd = InvalidValue;
    return 0 == rc;
}

bool FileDescriptorTraits::isValid( ValueType fd )
{
    return (InvalidValue != fd);
}

} // namespace Durin
