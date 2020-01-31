#ifndef DURIN_SCOPED_SCOED_FILE_DESCRIPTOR_HPP
#define DURIN_SCOPED_SCOED_FILE_DESCRIPTOR_HPP

#include <Durin/Config/Config.hpp>

#include <Durin/Scoped/BasicScoped.hpp>
#include <Durin/Type/FileDescriptor.hpp>

namespace Durin {

using ScopedFileDescriptor = BasicScopedObject<FileDescriptorTraits>;

} // namespace Durin

#endif // End of include guard
