#ifndef DURIN_TYPE_BASIC_TYPES_HPP
#define DURIN_TYPE_BASIC_TYPES_HPP

#include <Durin/Config/Config.hpp>

#ifdef DURIN_OS_WINDOWS
    #include <Durin/Type/BasicTypesWin32.hpp>
#else
    #include <Durin/Type/BasicTypesPosix.hpp>
#endif

#endif // End of include guard
