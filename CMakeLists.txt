cmake_minimum_required (VERSION 3.13.0)
cmake_policy(VERSION 3.13.0)

project(3DEngine VERSION 0.1)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_NO_GLU ")

enable_language(CXX)
set (CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(CUDA)

set(LIBRARY pthread X11 Xi dl GLEW glfw)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

add_library(lodepng "${PROJECT_DIR}/libraries/lodepng/lodepng.cpp")
target_include_directories(lodepng PUBLIC ${PROJECT_DIR}/lib/lodepng/)

add_subdirectory(Project.Engine/)
add_subdirectory(Project.Game/)
add_subdirectory(Project.UnitTests/)

file(COPY ${PROJECT_DIR}/resources DESTINATION ${EXECUTABLE_OUTPUT_PATH})