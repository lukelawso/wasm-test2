cmake_minimum_required(VERSION 3.10)
project(WasmTest2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Emscripten compile flags
set(CMAKE_EXECUTABLE_SUFFIX ".html")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.cc")

# Add compile flags for Emscripten
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file assets")

# Set Emscripten linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html")

# Add the executable
add_executable(game ${SOURCE_FILES})

# Create assets directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)

execute_process(COMMAND em++ --cflags
    OUTPUT_VARIABLE EM_CFLAGS)

set_target_properties(game PROPERTIES COMPILE_FLAGS "${EM_CFLAGS}")
