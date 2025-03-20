cmake_minimum_required(VERSION 3.10)
project(WasmTest2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.cc")

# Create assets directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)

# Check if we're using Emscripten
if(EMSCRIPTEN)
    # Emscripten/Web platform
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap']")
    
    # Set Emscripten linker flags
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html")
    
    # Get Emscripten compiler flags
    execute_process(COMMAND em++ --cflags
                    OUTPUT_VARIABLE EM_CFLAGS)
    
    # Add the executable with Emscripten settings
    add_executable(game_web ${SOURCE_FILES})
    set_target_properties(game_web PROPERTIES COMPILE_FLAGS "${EM_CFLAGS}")
    
    # Define web platform
    target_compile_definitions(game_web PRIVATE PLATFORM_WEB)
    
elseif(WIN32)

    # Find SDL2
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}\\external\\SDL2\\cmake" CACHE PATH "Path to SDL2 installation")

    set(SDL2_INC "${CMAKE_SOURCE_DIR}\\external\\SDL2\\include" CACHE PATH "Path to SDL2 includes")
    set(SDL2_LIB "${CMAKE_SOURCE_DIR}\\external\\SDL2\\lib\\x64\\SDL2.lib" CACHE PATH "Path to SDL2 libs")
    set(SDL2_LIB_MAIN "${CMAKE_SOURCE_DIR}\\external\\SDL2\\lib\\x64\\SDL2main.lib" CACHE PATH "Path to SDL2 main lib")

    # message(STATUS "hello hello!!${SDL2_DIR} ${CMAKE_SOURCE_DIR}")
    find_package(SDL2 REQUIRED)

    # Add executable
    add_executable(game ${SOURCE_FILES})

    # Include SDL2 directories
    target_include_directories(game PRIVATE ${SDL2_INC})
    target_link_libraries(game ${SDL2_LIB} ${SDL2_LIB_MAIN})
    
    # target_include_directories(game PRIVATE ${SDL2_INCLUDE_DIRS})
    # target_link_libraries(game ${SDL2_LIBRARIES})

    # target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}\\external\\SDL2\\include")
    # target_link_libraries(game PRIVATE "${CMAKE_SOURCE_DIR}\\external\\SDL2\\lib\\x64\\SDL2.lib")

    # Define Windows platform
    target_compile_definitions(game PRIVATE PLATFORM_WINDOWS PRIVATE WIN32)

    # Set working directory for debugging
    set_property(TARGET game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

    message(STATUS "")
    message(STATUS "Configuration Summary:")
    message(STATUS "---------------------")
    message(STATUS "Platform: Windows (MSVC)")
    message(STATUS "SDL2 Directory: ${SDL2_DIR}")
    message(STATUS "SDL2 Includes: ${SDL2_INC}")
    message(STATUS "SDL2 Lib: ${SDL2_LIB}")
    message(STATUS "---------------------")
    # GET_DIRECTORY_PROPERTY(output INCLUDE_DIRECTORIES)     
    # MESSAGE(WARNING ${output})
    # message(STATUS ${SDL2_LIB})
    
else()
    # Native Linux/WSL2 build settings
    message(STATUS "Configuring for native Linux build")
    
    # Find SDL2 package
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    
    # Add the executable for native build
    add_executable(game_native ${SOURCE_FILES})
    
    # Link against SDL2 for native build
    target_link_libraries(game_native ${SDL2_LIBRARIES})
    
    # Define Linux platform
    target_compile_definitions(game_native PRIVATE PLATFORM_LINUX)
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "---------------------")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
if(EMSCRIPTEN)
    message(STATUS "Platform: Web (Emscripten)")
elseif(WIN32 AND NOT CMAKE_CROSSCOMPILING)
    message(STATUS "Platform: Windows (MSVC)")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_CROSSCOMPILING)
    message(STATUS "Platform: Windows (MinGW cross-compilation)")
else()
    message(STATUS "Platform: Linux/WSL2")
endif()
message(STATUS "---------------------")