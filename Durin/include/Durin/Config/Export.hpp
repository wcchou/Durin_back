#ifndef INCLUDE_CONFIG_EXPORT_HPP
#define INCLUDE_CONFIG_EXPORT_HPP

#include <Durin/Config/Config.hpp>

#if defined(DURIN_OS_WINDOWS) && defined(DURIN_BUILD_DLL)
    #undef DURIN_EXPORT
    #define DURIN_EXPORT __declspec(dllexport)
#endif

#endif // End of include guard
