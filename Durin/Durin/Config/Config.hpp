#ifndef DURIN_CONFIG_CONFIG_HPP
#define DURIN_CONFIG_CONFIG_HPP

//
// Output Variables
//
// DURIN_OS_[Windows|LINUX]
// DURIN_[x86|x64]
// DURIN_BUILD_[LIB|DLL]
//
// Output Constants
//
// DURIN_CALL_STD
// DURIN_EXPORT
//

////////////////////////////////////////////////////////////////////////////////
//
// 0. OS configuration
//
////////////////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #define DURIN_OS_WINDOWS
#else
    #define DURIN_OS_LINUX
#endif

#if defined(_WIN64)
    #define DURIN_X64
#elif defined(WIN32) || defined(_WIN32)
    #define DURIN_X86
#else
#endif

//
// Calling convention
//
#if defined(DURIN_OS_WINDOWS) && defined(DURIN_X86)
    #define DURIN_CALL_STD __stdcall
#else
    #define DURIN_CALL_STD
#endif

// Build Type
#if defined(DURIN_BUILD_LIB) && defined(DURIN_BUILD_DLL)
    #error "Use DURIN_BUILD_LIB and DURIN_BUILD_DLL at the same time is prohibited."
#endif

#if !defined(DURIN_BUILD_LIB) && !defined(DURIN_BUILD_DLL)
    #error "Please specify DURIN_BUILD_LIB or DURIN_BUILD_DLL."
#endif

//
// DLL export
//
#define DURIN_EXPORT

#if defined(DURIN_OS_WINDOWS) && defined(DURIN_BUILD_DLL)
    #undef DURIN_EXPORT
    #define DURIN_EXPORT __declspec(dllimport)
#endif

// Platform header files
#ifdef DURIN_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

#endif // End of include guard
