cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(filedesc_project VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the executable
add_executable(test test.cpp filedesc.cpp )
add_executable(xmltest testxml.cpp )

# Include current directory for headers
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

