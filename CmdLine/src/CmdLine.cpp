#include <Durin/Config/Config.hpp>

#include <include/Durin/Config/Export.hpp>

#include <Durin/CmdLine/CmdLine.hpp>

#include <boost/algorithm/string.hpp>

namespace Durin {
namespace CmdLine {

std::vector<std::string> CmdLineContext::getOptionValues(
    const char* optionName, const char* delimiter ) {
    std::string oneLineValues = getOptionValue<std::string>( optionName, "" );

    if ( nullptr == delimiter ) {
        return { oneLineValues };
    }

    std::vector<std::string> result;
    boost::split( result, oneLineValues, boost::is_any_of( ";," ) );
    return result;
}

} // namespace CmdLine
} // namespace Durin
