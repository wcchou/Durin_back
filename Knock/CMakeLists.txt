cmake_minimum_required(VERSION 3.10)

# common paths
set(src_folder ${PROJECT_SOURCE_DIR}/Knock/src)
set(include_folder ${PROJECT_SOURCE_DIR}/Knock/include)

set(header_files ${include_folder}/Config/Config.hpp)

set(src_files ${src_folder}/MainEntry.cpp)

add_executable(knock ${src_files} ${header_files})

target_link_libraries(
    knock PUBLIC durin )

target_include_directories(
    knock PUBLIC ${include_folder}
                 durin)