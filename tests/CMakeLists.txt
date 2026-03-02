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

add_executable(xmltest
    testxml.cpp      # moved into this test/ folder
    ../xmlnode.cpp   # source from the parent folder
)
target_include_directories(xmltest PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Reuse the root helper for warnings
enable_strict_warnings(testfd)
enable_strict_warnings(xmltest)

# Register with CTest
add_test(NAME testfd   COMMAND $<TARGET_FILE:testfd>)
add_test(NAME xmltest  COMMAND $<TARGET_FILE:xmltest>)

  add_test(
    NAME sqrtexcept:e2e:bash
    COMMAND bash "${CMAKE_SOURCE_DIR}/tests/sqrtexcept_test.sh" $<TARGET_FILE:sqrtexcept>
  )
  add_test(
    NAME sqrtexpected:e2e:bash
    COMMAND bash "${CMAKE_SOURCE_DIR}/tests/sqrtexpected_test.sh" $<TARGET_FILE:sqrtexpected>
  )

