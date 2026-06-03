
cmake_minimum_required(VERSION 3.28)

project(hello VERSION 1.0 LANGUAGES CXX)

# ---- Language standard (project-wide) ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For clangd/IDE integrations
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(lib)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE greet)

