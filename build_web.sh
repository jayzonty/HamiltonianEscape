#!/bin/sh

RAYLIB_PATH="/home/jayzon/Documents/Projects/JameGam15/Deps/raylib"
RAYLIB_LIB_PATH="${RAYLIB_PATH}/libraylib.a"
RAYLIB_INCLUDE_PATH="${RAYLIB_PATH}/src"

SOURCE_FILES="../Source/GameScene.cpp ../Source/LevelData.cpp ../Source/Main.cpp"

# Create build directory if it doesn't exist yet
if [ ! -d ./build_web ]; then
    mkdir build_web
fi

cp -r Resources build_web/Resources

cd build_web

emcc -o game.html ${SOURCE_FILES} -Os -Wall ${RAYLIB_LIB_PATH} -I ${RAYLIB_INCLUDE_PATH} -I ../Source -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --preload-file ./Resources
