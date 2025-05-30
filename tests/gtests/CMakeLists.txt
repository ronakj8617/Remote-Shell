# Add test executable
add_executable(
        gtests
        gtest_main.cpp
)

# Link GoogleTest and core_lib
target_link_libraries(gtests
        PRIVATE
        core_lib          # ✅ link the shared library
        GTest::gtest
        GTest::gtest_main
)

# Include project headers
target_include_directories(gtests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Register tests
include(GoogleTest)
gtest_discover_tests(gtests)

# Optional: XML output for CI/CD
add_test(NAME gtests COMMAND gtests --gtest_output=xml:${CMAKE_BINARY_DIR}/gtest_report.xml)
