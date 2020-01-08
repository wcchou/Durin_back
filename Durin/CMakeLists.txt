cmake_minimum_required(VERSION 3.10)

# Common paths
set(src_folder ${PROJECT_SOURCE_DIR}/Durin/src)
set(include_folder ${PROJECT_SOURCE_DIR}/Durin/include/Durin)
set(export_folder ${PROJECT_SOURCE_DIR}/Durin/Durin)

set(header_files ${export_folder}/Config/Config.hpp
                 ${export_folder}/System/ErrorCode.hpp
                 ${include_folder}/Config/Export.hpp)

set(src_files ${src_folder}/System/ErrorCode.cpp)

add_library(durin STATIC ${src_files} ${header_files})

target_include_directories(
    durin PUBLIC ${PROJECT_SOURCE_DIR}/Durin)