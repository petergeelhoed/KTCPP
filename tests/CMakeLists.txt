# This file lives in: ${sourceDir}/test/CMakeLists.txt
# The root already called include(CTest) and enable_testing(),
# so we only need to define targets and register tests here.

# Test executables (excluded from ALL by design)
add_executable(testfd
    test.cpp         # moved into this test/ folder
    ../filedesc.cpp  # source from the parent folder
)
target_include_directories(testfd PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."   # ../ points to the parent folder
    )
set_target_properties(testfd PROPERTIES EXCLUDE_FROM_ALL YES)

add_executable(xmltest
    testxml.cpp      # moved into this test/ folder
    ../xmlnode.cpp   # source from the parent folder
)
target_include_directories(xmltest PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

set_target_properties(xmltest PROPERTIES EXCLUDE_FROM_ALL YES)

# Reuse the root helper for warnings
enable_strict_warnings(testfd)
enable_strict_warnings(xmltest)

# Register with CTest
add_test(NAME testfd   COMMAND $<TARGET_FILE:testfd>)
add_test(NAME xmltest  COMMAND $<TARGET_FILE:xmltest>)

