cmake_minimum_required(VERSION 3.10)
project(SDL2-Sandbox)

# let CMake to know that we want to use C++11 features.
set(CMAKE_CXX_STANDARD 11)

# define where CMake looks for the SDL2 modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# define which SDL2 libraries we should find.
find_package(SDL2 REQUIRED)

# define where the source code files are.
file(GLOB SOURCES src/*.cpp)

# define which SDL2 specific include directories are available.
include_directories(
    ${SDL2_INCLUDE_DIR}
)

# define sources that are used to build an executable.
add_executable(sdl2_sandbox ${SOURCES})

# define which SDL2 libraries are being linked.
target_link_libraries(sdl2_sandbox 
    ${SDL2_LIBRARY}
)
