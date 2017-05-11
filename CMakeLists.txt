cmake_minimum_required (VERSION 2.6)
project (tetra-creative)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_MODULE_PATH cmake)

set (GCC_COMPILE_FLAGS "-std=c++11")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

# Look up SDL2 and add include dir to path
find_package (SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories(inc)

file(GLOB_RECURSE TC_SOURCES "src/*.cpp")

add_executable (tetra-creative ${TC_SOURCES})
target_link_libraries(tetra-creative ${SDL2_LIBRARY})
