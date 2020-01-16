cmake_minimum_required(VERSION 3.10)

# Common paths
set(src_folder ${PROJECT_SOURCE_DIR}/CmdLine/src)
set(include_folder ${PROJECT_SOURCE_DIR}/CmdLine/include/Durin/CmdLine)
set(export_folder ${PROJECT_SOURCE_DIR}/CmdLine/Durin/CmdLine)

set(header_files ${export_folder}/Commands.hpp
                 ${export_folder}/CmdLine.hpp)

set(src_files ${src_folder}/Commands.cpp
              ${src_folder}/CmdLine.cpp )

add_library(durin_cmdline STATIC ${src_files} ${header_files})

target_link_libraries(
    durin_cmdline PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                          durin
                          ${LOCAL_LIB}/libboost_program_options-gcc7-mt-d-x64-1_70.a
                          ${LOCAL_LIB}/libboost_filesystem-gcc7-mt-d-x64-1_70.a)

target_include_directories(
    durin_cmdline PUBLIC ${PROJECT_SOURCE_DIR}/CmdLine
                         ${LOCAL_INCLUDE}/boost-1_70
                  PRIVATE ${PROJECT_SOURCE_DIR}/Durin )