#ifndef DURIN_TYPE_WIN32_BASIC_TYPES_HPP
#define DURIN_TYPE_WIN32_BASIC_TYPES_HPP

#include <Durin/Config/Config.hpp>

#ifndef DURIN_OS_LINUX
    #error Define DURIN_OS_LINUX before including this file
#endif

#include <cstdint>

namespace Durin {

using kbyte = unsigned char;

using kint8  = std::int8_t;
using kint16 = std::int16_t;
using kint32 = std::int32_t;
using kint64 = std::int64_t;

using kuint8  = std::uint8_t;
using kuint16 = std::uint16_t;
using kuint32 = std::uint32_t;
using kuint64 = std::uint64_t;

} // namespace Durin

#endif // End of include guard
