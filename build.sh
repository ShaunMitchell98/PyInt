#!/bin/bash

# PyInt build script for macOS/Linux

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

echo ""
echo "Build completed. To run PyInt:"
echo "./PyInt.Console/pyint"