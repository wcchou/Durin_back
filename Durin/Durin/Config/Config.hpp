#ifndef DURIN_CONFIG_CONFIG_HPP
#define DURIN_CONFIG_CONFIG_HPP

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 0. OS configuration
//
////////////////////////////////////////////////////////////////////////////////////////////////////
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

//
// DLL export
//
#ifdef DURIN_OS_WINDOWS
    #define DURIN_EXPORT __declspec(import)
#else
    #define DURIN_EXPORT
#endif

// platform header files
#ifdef DURIN_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

#endif // End of include guard
