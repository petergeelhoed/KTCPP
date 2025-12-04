
cmake_minimum_required(VERSION 3.10)

project(filedesc_project VERSION 1.0 LANGUAGES CXX)

# Project-wide C++ standard (per target via property is recommended)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json (also set via presets below for robustness)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Targets ----
add_executable(test       test.cpp       filedesc.cpp)
add_executable(xmltest    xmlnode.cpp    testxml.cpp)
add_executable(sound      sound.cpp)
add_executable(gather     gather.cpp)
add_executable(algorithm  algorithm.cpp)
add_executable(intpromotion intpromotion.cpp)
add_executable(fibonacci  fibonacci.cpp)

# Include dirs for 'test'
target_include_directories(test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Per-target warnings (xmltest had explicit options)
target_compile_options(xmltest PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)

# Optional: apply warnings to ALL executables via a helper function
function(enable_strict_warnings target)
    target_compile_options("${target}" PRIVATE
        -Wall -Wextra -Wpedantic -Werror -Wconversion
    )
endfunction()

# Apply to the ones you want (or all)
enable_strict_warnings(test)
enable_strict_warnings(sound)
enable_strict_warnings(gather)
enable_strict_warnings(algorithm)
enable_strict_warnings(intpromotion)
enable_strict_warnings(fibonacci)

