cmake_minimum_required (VERSION 3.13.0)
cmake_policy(VERSION 3.13.0)

project(3DEngine VERSION 0.1)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_NO_GLU -pthread")

enable_language(CXX)
set (CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(CUDA)

if(WIN32)
    set(LIBRARY glfw)
else()
    set(LIBRARY X11 Xi dl glfw)
    set(PHYSX_LIBRARIES PhysX_static_64 PhysXCommon_static_64 PhysXExtensions_static_64 PhysXFoundation_static_64 PhysXPvdSDK_static_64 PhysXCooking_static_64 PhysXGpu_64)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc")
endif()

set(GLEW_INCLUDE_DIRS ${PROJECT_DIR}/libraries/External/glew-2.1.0/include/)


add_subdirectory(${PROJECT_DIR}/libraries/External/glew-2.1.0/build/cmake/)
add_subdirectory(Project.Engine/)
add_subdirectory(Project.Game/)
add_subdirectory(Project.UnitTests/)

file(COPY ${PROJECT_DIR}/resources DESTINATION ${EXECUTABLE_OUTPUT_PATH})