cmake_minimum_required(VERSION 3.10)

# Project name
project(JameGam15)

find_package(raylib 3.0 REQUIRED)
find_package(Threads REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile_commands.json for YouCompleteMe (YCM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set SOURCES to contain all the source files
set(SOURCES
    Source/GameScene.cpp
    Source/Main.cpp
)

# Executable
add_executable(JameGam15 ${SOURCES})

include_directories(Source)

target_compile_options(JameGam15 PUBLIC -Wall)

# Link libraries
target_link_libraries(JameGam15 raylib dl Threads::Threads)

# Post-build copy command
#add_custom_command(TARGET ResourceGame POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:JameGam15>/resources/
#)
