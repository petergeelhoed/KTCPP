
cmake_minimum_required(VERSION 3.10)

project(filedesc_project VERSION 1.0 LANGUAGES CXX)

# ---- Language standard (project-wide) ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For clangd/IDE integrations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Helper: create executable and register it in a list ----
# Usage: add_app(<name> <src1> <src2> ...)
set(ALL_APPS "")  # will hold all target names

function(add_app name)
    add_executable("${name}" ${ARGN})
    # Remember the target name in a project-wide variable
    list(APPEND ALL_APPS "${name}")
    set(ALL_APPS "${ALL_APPS}" PARENT_SCOPE)
endfunction()

# ---- Define targets (sources per target) ----
add_app(test       test.cpp       filedesc.cpp)
add_app(xmltest    xmlnode.cpp    testxml.cpp)
add_app(sound      sound.cpp)
add_app(gather     gather.cpp)
add_app(algorithm  algorithm.cpp)
add_app(intpromotion intpromotion.cpp)
add_app(fibonacci  fibonacci.cpp)

# ---- Common warnings helper ----
function(enable_strict_warnings target)
    target_compile_options("${target}" PRIVATE
        -Wall -Wextra -Wpedantic -Werror -Wconversion
    )
endfunction()

# ---- Apply common settings to all targets in the list ----
foreach(tgt IN LISTS ALL_APPS)
    enable_strict_warnings("${tgt}")
endforeach()

# ---- Per-target special cases ----
# Include dirs for 'test' only
target_include_directories(test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

