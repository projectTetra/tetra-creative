cmake_minimum_required (VERSION 2.6)
project (tetra-creative)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_MODULE_PATH cmake)

# Look up SDL2 and add include dir to path
find_package (SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable (tetra-creative src/main.cpp)
target_link_libraries(tetra-creative ${SDL2_LIBRARY})
