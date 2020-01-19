cmake_minimum_required(VERSION 3.10)

# common paths
set(src_folder ${PROJECT_SOURCE_DIR}/Knock/src)
set(include_folder ${PROJECT_SOURCE_DIR}/Knock/include)

set(header_files ${include_folder}/Config/Config.hpp
                 ${include_folder}/Commands.hpp
                 ${include_folder}/Task/HttpServer.hpp
                 ${include_folder}/Task/FileGenerator.hpp
                 ${include_folder}/Task/ProcessUtil.hpp)

set(src_files ${src_folder}/MainEntry.cpp
              ${src_folder}/Commands.cpp
              ${src_folder}/Task/HttpServer.cpp
              ${src_folder}/Task/FileGenerator.cpp
              ${src_folder}/Task/ProcessUtil.cpp)

add_executable(knock ${src_files} ${header_files})

find_package(Threads)

target_link_libraries(
    knock PUBLIC ${CMAKE_THREAD_LIBS_INIT}
                 ${LOCAL_LIB}/yaml-cpp/libyaml-cpp.a
                 durin
                 durin_cmdline)

target_include_directories(
    knock PUBLIC ${include_folder}
                 ${LOCAL_INCLUDE}/yaml-cpp/include/
                 durin
                 durin_cmdline)