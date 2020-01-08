#ifndef INCLUDE_CONFIG_EXPORT_HPP
#define INCLUDE_CONFIG_EXPORT_HPP

#include <Durin/Config/Config.hpp>

#ifdef DURIN_OS_WINDOWS
    #define DURIN_EXPORT __declspec(export)
#else
    #define DURIN_EXPORT
#endif

#endif // End of include guard
