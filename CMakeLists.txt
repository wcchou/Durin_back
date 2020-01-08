cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Durin)

if (NOT DEFINED LOCAL_LIB)
    message(FATAL_ERROR "Please specify LOCAL_LIB via -DLOCAL_LIB=local-lib-path. E.g. /usr/lib")
endif()

if (NOT DEFINED LOCAL_INCLUDE)
    message(FATAL_ERROR "Please specify LOCAL_INCLUDE via -DLOCAL_INCLUDE=local-include-path. E.g. /usr/include")
endif()

add_subdirectory(Durin)
add_subdirectory(test)
add_subdirectory(Knock)