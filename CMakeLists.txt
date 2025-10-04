# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)

# Define the project
project(
    Cla
    VERSION 1.0
    DESCRIPTION "Cells Authomaton"
    LANGUAGES C
)
# Set the C standart to C
set(CMAKE_C_STANDART 99)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)
# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(Cla
    main.c
)

# Link to the actual SDL3 library.
target_link_libraries(Cla PRIVATE SDL3::SDL3)