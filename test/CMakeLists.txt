cmake_minimum_required(VERSION 3.10)

add_executable(durin_test MainEntry.cpp)

find_package(Threads)

target_link_libraries(
    durin_test PRIVATE ${CMAKE_THREAD_LIBS_INIT}
                       durin
                       ${LOCAL_LIB}/libboost_unit_test_framework-gcc7-mt-d-x64-1_70.a)

target_include_directories(
    durin_test PRIVATE "${LOCAL_INCLUDE}/boost-1_70"
                       durin)