#include <Durin/String/String.hpp>

#include <include/Durin/Config/Export.hpp>

#include <Durin/String/String.hpp>

#include <cwctype>
#include <cctype>
#include <algorithm>

namespace Durin {
namespace String {

void lower( std::string& str )
{
    std::transform(
        str.begin(), str.end(),
        str.begin(), ::tolower );
}

void lower( std::wstring& str )
{
    std::transform(
        str.begin(), str.end(),
        str.begin(), std::towlower );
}

bool isDecimalNumber( const std::string& str )
{
    for ( auto ch : str ) {
        if ( !std::isdigit( ch ) ) {
            return false;
        }
    }

    return true;
}

bool isDecimalNumber( const std::wstring& str )
{
    for ( auto ch : str ) {
        if ( !std::iswdigit( ch ) ) {
            return false;
        }
    }

    return true;
}

} // namespace String
} // namespace String
