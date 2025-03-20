# Wasm Test 2 (name pending)

Toy bullet hell game intended to run in the browser.

## Features

## To add
A few things are planned for the game:
* Sprite loading
* level layout
* collision detection for various geometry types

## Requirements

SDL is required for everything. 

## Web Build
The wasm build is done using [empscripten](https://emscripten.org/docs/getting_started/downloads.html)

SDL comes with emscripten. For linux `sudo apt install libsdl2-dev`

## Windows build (MSVC)
I develop everything in a WSL2 environment, but sometimes like to do visual debugging with Visual Studio. In this case you can set up a solution file targetting the WSL directory, and build the files on the windows side using MSVC.

To build for windows using MSVC, you'll need to get the SDL2 dev libraries from [https://github.com/libsdl-org/SDL/releases](here). Extract this file into `eternal/SDL2`.

From there, running the build script targetting windows should prouduce a solution file suitable for opening in visual studio.