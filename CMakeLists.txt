cmake_minimum_required (VERSION 3.1)
project (tetra-creative)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_MODULE_PATH cmake)

set (GCC_COMPILE_FLAGS "-std=c++1z")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")


set (ASSET_ROOT ${CMAKE_BINARY_DIR}/assets)
configure_file ("./metasrc/AssetRoot.h.in" "./lib/inc/AssetRoot.h")

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package (SDL2 REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(lib/inc)

add_subdirectory (lib)

add_executable(firstTriangle sketches/firstTriangle.cpp)
target_link_libraries(firstTriangle tcCore)
target_link_libraries(firstTriangle ${SDL2_LIBRARY})
target_link_libraries(firstTriangle ${OPENGL_LIBRARIES})
target_link_libraries(firstTriangle ${GLEW_LIBRARY})

