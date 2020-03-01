#ifndef DURIN_TRACE_TRACE_HPP
#define DURIN_TRACE_TRACE_HPP

#include <Durin/Config/Config.hpp>

#include <sstream>

namespace Durin {
namespace Trace {

class DURIN_EXPORT MessagePack {
public:
    ~MessagePack();

    template <typename Value>
    MessagePack& operator <<( const Value& value )
    {
        oss_ << value;
        return *this;
    }

private:
    std::ostringstream  oss_;
};

} // namespace Trace
} // namespace Durin

#ifdef DURIN_TRACE
# define TRACE(...) \
    { Durin::Trace::MessagePack() << __VA_ARGS__; }
#else
# define TRACE(...)
#endif

#endif // End of include guard
