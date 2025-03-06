# PyInt Console Application CMake file

# Add the main executable source
set(CONSOLE_SOURCES
    main.c
)

# Create the console executable
add_executable(PyInt.Console ${CONSOLE_SOURCES})

# Link against the PyInt library
target_link_libraries(PyInt.Console PRIVATE PyInt)

# Define platform-specific macros
if(APPLE)
    target_compile_definitions(PyInt.Console PRIVATE MACOS=1)
elseif(UNIX)
    target_compile_definitions(PyInt.Console PRIVATE LINUX=1)
elseif(WIN32)
    target_compile_definitions(PyInt.Console PRIVATE WIN32=1)
endif()

# Add debug definitions if in debug mode
target_compile_definitions(PyInt.Console PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Ensure console app can find the platform compatibility layer
target_include_directories(PyInt.Console PRIVATE ${CMAKE_SOURCE_DIR}/compat)

# Make sure the final executable is named "pyint"
set_target_properties(PyInt.Console PROPERTIES OUTPUT_NAME "pyint")