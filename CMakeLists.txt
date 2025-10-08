# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.23)

# Define the project
project(
    Cla
    VERSION 1.0
    DESCRIPTION "Cells Authomaton"
    LANGUAGES C
)

# Set the C standart to C
set(CMAKE_C_STANDART 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(
  CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)

set(
  CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)

add_executable(Cla
  main.c
)
if(APPLE)
  set_target_properties(Cla PROPERTIES
    INSTALL_RPATH "@executable_path;@loader_path"
    BUILD_WITH_INSTALL_RPATH TRUE
    MACOSX_RPATH TRUE
  )
elseif(UNIX)
  set_target_properties(Cla PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
endif()

if(WIN32)
  set(MATH_LIBRARY "" CACHE STRING "" FORCE)
endif()


# Set a variable for our vendor directory
set(SDLTTF_VENDORED ON)
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor") 

# Add the SDL libraries from our submodules
add_subdirectory(${VENDOR_DIR}/SDL) 
add_subdirectory(${VENDOR_DIR}/EGL)

target_link_libraries(Cla PRIVATE SDL3::SDL3)
target_link_libraries(Cla PRIVATE EGL::EGL)