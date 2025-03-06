cmake_minimum_required(VERSION 3.10)
project(PyInt VERSION 1.0.0 LANGUAGES C)

# C standard to use
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build options
option(PYINT_BUILD_TESTS "Build the PyInt test suite" OFF)

# Define platform-specific preprocessor macros
if(APPLE)
    add_definitions(-DMACOS)
elseif(UNIX)
    add_definitions(-DLINUX)
elseif(WIN32)
    add_definitions(-DWIN32)
endif()

# Add cross-platform compatibility layer
include_directories(${CMAKE_SOURCE_DIR}/compat)

# Add subdirectories
add_subdirectory(PyInt)
add_subdirectory(PyInt.Console)

# Add tests if enabled
if(PYINT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()