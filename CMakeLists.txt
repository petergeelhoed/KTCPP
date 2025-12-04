
cmake_minimum_required(VERSION 3.10)

project(filedesc_project VERSION 1.0 LANGUAGES CXX)

# ---- Language standard (project-wide) ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For clangd/IDE integrations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Lists to hold target names ----
set(APPS "")       # regular app targets
set(TEST_APPS "")      # test-only targets (excluded from ALL)

# ---- Helper: create regular executable and register it ----
# Usage: add_app(<name> <src1> <src2> ...)
function(add_app name)
    add_executable("${name}" ${ARGN})
    list(APPEND APPS "${name}")
    set(APPS "${APPS}" PARENT_SCOPE)
endfunction()

# ---- Helper: create test executable, EXCLUDE_FROM_ALL, and register it ----
# Usage: add_test_app(<name> <src1> <src2> ...)
function(add_test_app name)
    add_executable("${name}" ${ARGN})
    # Do not build by default (excluded from 'all')
    set_target_properties("${name}" PROPERTIES EXCLUDE_FROM_ALL YES)
    list(APPEND TEST_APPS "${name}")
    set(TEST_APPS "${TEST_APPS}" PARENT_SCOPE)
endfunction()

# ---- Define targets ----
# Test-only apps (excluded from ALL)
add_test_app(testfd       test.cpp       filedesc.cpp)
add_test_app(xmltest    xmlnode.cpp    testxml.cpp)

# Regular apps
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

# ---- Apply common settings to all targets ----
foreach(tgt IN LISTS APPS TEST_APPS)
    enable_strict_warnings("${tgt}")
endforeach()

# ---- Per-target special cases ----
# Include dirs for 'test' only
target_include_directories(testfd PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

