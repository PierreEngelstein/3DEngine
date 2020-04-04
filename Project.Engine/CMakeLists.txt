file(GLOB_RECURSE ENGINE_SRC "src/*.cpp")

add_library(engine STATIC ${ENGINE_SRC})
target_include_directories(engine 
    PRIVATE 
        ${PROJECT_DIR}/Project.Engine/src/
        ${PROJECT_DIR}/libraries/
        ${PROJECT_DIR}/libraries/physx/include/
        ${PROJECT_DIR}/libraries/lodepng/
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)

target_link_directories(engine PUBLIC ${PROJECT_DIR}/libraries/)

target_link_libraries(engine PUBLIC ${OPENGL_LIBRARIES} ${LIBRARY} ${GLEW_LIBRARIES} lodepng
    PhysX_static_64 PhysXCommon_static_64 PhysXExtensions_static_64 PhysXFoundation_static_64 PhysXPvdSDK_static_64  PhysXCooking_static_64
    PhysXGpu_64
    cuda dl)