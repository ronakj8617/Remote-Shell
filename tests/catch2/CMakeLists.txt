# tests/CMakeLists.txt
find_package(Catch2 REQUIRED)

add_executable(tests
        test_main.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
add_test(NAME catch2_xml COMMAND tests -r junit -o ${CMAKE_BINARY_DIR}/catch2_report.xml)