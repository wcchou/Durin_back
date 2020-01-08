#ifndef DURIN_SYSTEM_ERROR_CODE_HPP
#define DURIN_SYSTEM_ERROR_CODE_HPP

#include <Durin/Config/Config.hpp>

namespace Durin {

#ifdef DURIN_OS_WINDOWS
using NativeErrorCode = DWORD;
#else
using NativeErrorCode = int;
#endif

DURIN_EXPORT NativeErrorCode DURIN_CALL_STD lastErrorCode();

} // namespace Durin

#endif // End of include guard
