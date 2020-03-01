cmake_minimum_required(VERSION 3.10)

# Common paths
set(src_folder ${PROJECT_SOURCE_DIR}/Durin/src)
set(include_folder ${PROJECT_SOURCE_DIR}/Durin/include/Durin)
set(export_folder ${PROJECT_SOURCE_DIR}/Durin/Durin)

set(header_files ${export_folder}/Config/Config.hpp
                 ${export_folder}/Trace/Trace.hpp
                 ${export_folder}/Type/BasicTypes.hpp
                 ${export_folder}/Type/FileDescriptor.hpp
                 ${export_folder}/System/ErrorCode.hpp
                 ${export_folder}/Scoped/BasicScoped.hpp
                 ${export_folder}/Scoped/ScopedFileDescriptor.hpp
                 ${export_folder}/String/String.hpp
                 ${export_folder}/Hash/Hash.hpp
                 ${include_folder}/Config/Export.hpp)

if (WIN32)
    list(APPEND header_files ${export_folder}/Type/BasicTypesWin32.hpp)
else()
    list(APPEND header_files ${export_folder}/Type/BasicTypesPosix.hpp)
endif()

set(src_files ${src_folder}/Trace/Trace.cpp
              ${src_folder}/Type/FileDescriptor.cpp
              ${src_folder}/System/ErrorCode.cpp
              ${src_folder}/String/String.cpp
              ${src_folder}/Hash/Hash.cpp)

add_library(durin STATIC ${src_files} ${header_files})

find_package(OpenSSL REQUIRED)
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "can't found openssl")
endif()

target_include_directories(
    durin PUBLIC ${OPENSSL_INCLUDE_DIR}
                 ${LOCAL_INCLUDE}/boost-1_70
                 ${PROJECT_SOURCE_DIR}/Durin)

target_link_libraries(
    durin PUBLIC ${OPENSSL_LIBRARIES}
                 ${LOCAL_LIB}/libboost_filesystem-gcc7-mt-d-x64-1_70.a)