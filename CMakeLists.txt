
cmake_minimum_required(VERSION 3.28)

project(filedesc_project VERSION 1.0 LANGUAGES CXX)

# Enable CTest integration
include(CTest)
enable_testing()

# ---- Language standard (project-wide) ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For clangd/IDE integrations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Lists to hold target names ----
set(APPS "")       # regular app targets

# ---- Helper: create regular executable and register it ----
# Usage: add_app(<name> <src1> <src2> ...)
function(add_app name)
    add_executable("${name}" ${ARGN})
    list(APPEND APPS "${name}")
    set(APPS "${APPS}" PARENT_SCOPE)
endfunction()

# ---- Common warnings helper ----
function(enable_strict_warnings target)
    target_compile_options("${target}" PRIVATE
        -Wall -Wextra -Wpedantic -Werror -Wconversion
    )
endfunction()

# ---- Regular apps ----
add_app(sound        sound.cpp)
add_app(gather       gather.cpp)
add_app(algorithm    algorithm.cpp)
add_app(intpromotion intpromotion.cpp)
add_app(fibonacci    fibonacci.cpp)
add_app(iterator     iterator.cpp)

# ---- Bring in tests from subdirectory ----
add_subdirectory(tests)

# ---- Apply common settings to all regular targets ----
foreach(tgt IN LISTS APPS)
    enable_strict_warnings("${tgt}")
endforeach()


add_test(
  NAME iterator
  COMMAND bash -c "echo 'dsfj;adsf;' | \"$<TARGET_FILE:iterator>\""
)
set_tests_properties(iterator PROPERTIES
  PASS_REGULAR_EXPRESSION "dsfj[\r\n]+adsf"
)

add_test(
  NAME iteratorspaces
  COMMAND bash -c "echo 'a ; b;c' | \"$<TARGET_FILE:iterator>\""
)
set_tests_properties(iteratorspaces PROPERTIES
  PASS_REGULAR_EXPRESSION "a [\r\n]+ b[\r\n]+c"
)

