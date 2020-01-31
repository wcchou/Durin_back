#ifndef DURIN_TYPE_FILE_DESCRIPTOR_HPP
#define DURIN_TYPE_FILE_DESCRIPTOR_HPP

#include <Durin/Config/Config.hpp>

namespace Durin {

struct DURIN_EXPORT FileDescriptorTraits {
    using ValueType = int;

    static bool close( ValueType& fd );
    static bool isValid( ValueType fd );

    constexpr static ValueType InvalidValue = -1;
};

} // namespace Durin

#endif // End of include guard
