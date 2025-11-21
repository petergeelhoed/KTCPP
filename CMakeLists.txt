cmake_minimum_required(VERSION 3.10)

project(filedesc_project VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test test.cpp filedesc.cpp)
add_executable(xmltest xmlnode.cpp testxml.cpp)
add_executable(sound sound.cpp)
add_executable(gather gather.cpp)
add_executable(algorithm algorithm.cpp)
add_executable(intpromotion intpromotion.cpp)

target_compile_options(xmltest PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
