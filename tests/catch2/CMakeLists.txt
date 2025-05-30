# Ensure Catch2 is available
find_package(Catch2 REQUIRED)

# Define Catch2 test executable
add_executable(
        catch2_tests
        test_main.cpp
)

# Link Catch2 and your main library
target_link_libraries(catch2_tests
        PRIVATE
        core_lib                   # ✅ Required for linking CommandExecutor, etc.
        Catch2::Catch2WithMain
)

# Include project headers
target_include_directories(catch2_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Register with CTest
include(Catch)
catch_discover_tests(catch2_tests)

# Optional: XML report for CI pipelines
add_test(NAME catch2_xml COMMAND catch2_tests -r junit -o ${CMAKE_BINARY_DIR}/catch2_report.xml)
