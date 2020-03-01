#include <Durin/Config/Config.hpp>
#include <include/Durin/Config/Export.hpp>

#include <Durin/Trace/Trace.hpp>

#include <iostream>

namespace Durin {
namespace Trace {

MessagePack::~MessagePack()
{
    std::cout << oss_.str() << std::endl;
}

} // namespace Trace
} // namespace Durin