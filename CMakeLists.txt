cmake_minimum_required (VERSION 3.1)
project (tetra-creative)

message ("source: " ${CMAKE_CURRENT_SOURCE_DIR})
message ("binary: " ${CMAKE_CURRENT_BINARY_DIR})

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_MODULE_PATH cmake)

set (GCC_COMPILE_FLAGS "-std=c++1z")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

set (ASSET_ROOT ${CMAKE_BINARY_DIR}/assets)
configure_file ("./metasrc/AssetRoot.h.in" "./inc/AssetRoot.h")

find_package (OpenGL REQUIRED)

find_package (GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Look up SDL2 and add include dir to path
find_package (SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories(inc)

file(GLOB_RECURSE TC_SOURCES "inc/*.hpp" "src/*.cpp")

add_executable (tetra-creative ${TC_SOURCES})
target_link_libraries(tetra-creative ${SDL2_LIBRARY})
target_link_libraries(tetra-creative ${OPENGL_LIBRARIES})
target_link_libraries(tetra-creative ${GLEW_LIBRARY})

