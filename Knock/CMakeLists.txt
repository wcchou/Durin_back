cmake_minimum_required(VERSION 3.10)

# common paths
set(src_folder ${PROJECT_SOURCE_DIR}/Knock/src)
set(include_folder ${PROJECT_SOURCE_DIR}/Knock/include)

set(header_files ${include_folder}/Config/Config.hpp
                 ${include_folder}/Commands.hpp
                 ${include_folder}/Task/HttpServer.hpp)

set(src_files ${src_folder}/MainEntry.cpp
              ${src_folder}/Commands.cpp
              ${src_folder}/Task/HttpServer.cpp)

add_executable(knock ${src_files} ${header_files})

find_package(Threads)

target_link_libraries(
    knock PUBLIC ${CMAKE_THREAD_LIBS_INIT}
                 durin
                 durin_cmdline)

target_include_directories(
    knock PUBLIC ${include_folder}
                 durin
                 durin_cmdline)