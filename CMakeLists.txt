cmake_minimum_required(VERSION 3.14)
project(RemoteShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
include(CTest)
enable_testing()

include(FetchContent)

# ====================================== #
# Fetch Crow (Amalgamated version)       #
# ====================================== #
set(CROW_AMALGAMATE ON)
FetchContent_Declare(
        Crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG master
)
set(CROW_AMALGAMATE ON)
set(CROW_BUILD_EXAMPLES OFF)
set(CROW_BUILD_TESTS OFF)
FetchContent_MakeAvailable(Crow)

# ======================================
# Fetch GoogleTest (GTest + GMock)
# ======================================
FetchContent_Declare(
        GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(GTest)

# Add GTest test directory
add_subdirectory(tests/gtests)

# ======================================
# Fetch Catch2
# ======================================
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(tests/catch2)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(remoteshell
        src/main.cpp
        src/server/server.cpp
        src/client/client.cpp
        src/handle_signals.cpp
        include/cors.h
)

# Update include directories
target_link_libraries(remoteshell PRIVATE
        Boost::system
        Boost::thread
)

target_include_directories(remoteshell PRIVATE
        ${Boost_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Doxygen Documentation setup
find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

    add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )
else()
    message(STATUS "Doxygen not found, 'doc_doxygen' target will not be available")
endif()