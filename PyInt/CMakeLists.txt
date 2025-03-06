# PyInt Library CMake file
set(PYINT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Create a list of all source files recursively
file(GLOB_RECURSE PYINT_SOURCES
    "${PYINT_SOURCE_DIR}/*.c"
)

# Create a list of all header files recursively
file(GLOB_RECURSE PYINT_HEADERS
    "${PYINT_SOURCE_DIR}/*.h"
)

# Create the PyInt library
add_library(PyInt STATIC ${PYINT_SOURCES} ${PYINT_HEADERS})

# Set include directories
target_include_directories(PyInt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Compilation
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Services
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Types
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Virtual Machine
)

# Define platform-specific macros
if(APPLE)
    target_compile_definitions(PyInt PRIVATE MACOS=1)
elseif(UNIX)
    target_compile_definitions(PyInt PRIVATE LINUX=1)
elseif(WIN32)
    target_compile_definitions(PyInt PRIVATE WIN32=1)
endif()

# Add debug definitions if in debug mode
target_compile_definitions(PyInt PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Ensure PyInt can find the platform compatibility layer
target_include_directories(PyInt PRIVATE ${CMAKE_SOURCE_DIR}/compat)