#ifndef DURIN_STRING_STRING_HPP
#define DURIN_STRING_STRING_HPP

#include <Durin/Config/Config.hpp>

#include <string>

namespace Durin {
namespace String {

DURIN_EXPORT void lower( std::string& str );
DURIN_EXPORT void lower( std::wstring& str );

DURIN_EXPORT bool isDecimalNumber( const std::string& str );
DURIN_EXPORT bool isDecimalNumber( const std::wstring& str );

} // namespace String
} // namespace Durin

#endif // End of include guard
