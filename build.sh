#!/bin/bash

# Make sure the Emscripten environment is activated
# Uncomment and modify the line below to point to your Emscripten installation if needed
# source /path/to/emsdk/emsdk_env.sh

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
emcmake cmake ..

# Build
emmake make

# Serve the result (optional)
echo "Build completed. You can run a local server with:"
echo "cd build && python3 -m http.server"
