#include <Durin/Config/Config.hpp>
#include <include/Durin/Config/Export.hpp>

#include <Durin/System/ErrorCode.hpp>

#ifdef DURIN_OS_LINUX
#include <errno.h>
#endif

namespace Durin {

NativeErrorCode lastErrorCode()
{
#ifdef DURIN_OS_WINDOWS
    return ::GetLastError();
#else
    return errno;
#endif
}

} // namespace Durin
