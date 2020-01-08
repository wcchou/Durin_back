#ifndef DURIN_TYPE_WIN32_BASIC_TYPES_HPP
#define DURIN_TYPE_WIN32_BASIC_TYPES_HPP

#include <Durin/Config/Config.hpp>

#ifndef DURIN_OS_WINDOWS
    #error Define DURIN_OS_WINDOWS before including this file
#endif

namespace Durin {

using kbyte = unsigned char;

using kint8  = __int8;
using kint16 = __int16;
using kint32 = __int32;
using kint64 = __int64;

using kuint8  = unsigned __int8;
using kuint16 = unsigned __int16;
using kuint32 = unsigned __int32;
using kuint64 = unsigned __int64;

} // namespace Durin

#endif // End of include guard
