#!/bin/bash

set -e

# Check for platform argument
if [ $# -lt 1 ]; then
    echo "Usage: $0 [web|native|windows]"
    echo "  web     - Build for web using Emscripten"
    echo "  native  - Build natively for WSL2/Linux"
    echo "  windows - Cross-compile for Windows from WSL2"
    exit 1
fi

# Platform selection
PLATFORM=$1
BUILD_TYPE="Release"
if [ "$PLATFORM" = "windows" ]; then
    BUILD_TYPE="Debug"
fi

# Override build type if specified
if [ $# -ge 2 ]; then
    if [ "$2" = "debug" ]; then
        BUILD_TYPE="Debug"
    elif [ "$2" = "release" ]; then
        BUILD_TYPE="Release"
    else
        echo "Invalid build type: $2"
        echo "Valid options are: debug, release"
        exit 1
    fi
fi

# Make sure the Emscripten environment is activated if building for web
if [ "$PLATFORM" = "web" ]; then
    # Uncomment and modify the line below to point to your Emscripten installation if needed
    # source /path/to/emsdk/emsdk_env.sh
    echo "Building for web platform using Emscripten..."
    
    # Create build directory for web
    mkdir -p build_web
    cd build_web
    
    # Configure with CMake for web
    emcmake cmake ..
    
    # Build
    emmake make
    
    # Serve the result (optional)
    echo "Build completed. You can run a local server with:"
    echo "cd build_web && python3 -m http.server"
    
elif [ "$PLATFORM" = "native" ]; then
    echo "Building for native platform (WSL2/Linux)..."
    
    # Create build directory for native
    mkdir -p build_native
    cd build_native
    
    # Configure with CMake for native
    cmake ..
    
    # Build
    make -j$(nproc)
    
    # Output location of executable
    echo "Build completed. The executable is located at:"
    echo "$(pwd)/game_native"
    
elif [ "$PLATFORM" = "windows" ]; then
mkdir -p build_windows_msvc
WSL_PROJECT_PATH=$(wslpath -w $(pwd))
CXX_COMPILER=$(powershell.exe -Command "& { \
    & 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat' -no_logo \
    '&&' where.exe cl.exe | Select-Object -First 1 \
}")
echo "Found C++ compiler at: $CXX_COMPILER"

powershell.exe -Command "& { \
    & 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat' -no_logo \
    '&&' 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' \
    -S '$WSL_PROJECT_PATH' \
    -B '$WSL_PROJECT_PATH\build_windows_msvc' \
    -G 'Visual Studio 17 2022' \
    -DCMAKE_CXX_COMPILER='$CXX_COMPILER' \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS_DEBUG='/Zi /Ob0 /Od /RTC1' \
    -DCMAKE_EXE_LINKER_FLAGS_DEBUG='/DEBUG:FULL' \
}"

else
    echo "Invalid platform: $PLATFORM"
    echo "Valid options are: web, native, windows"
    exit 1
fi