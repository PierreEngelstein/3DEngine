file(GLOB_RECURSE ENGINE_SRC "src/*.cpp")

add_library(engine SHARED ${ENGINE_SRC})
target_include_directories(engine 
    PRIVATE 
        ${PROJECT_DIR}/Project.Engine/src/
        ${PROJECT_DIR}/libraries/
        ${PROJECT_DIR}/libraries/physx/include/
        ${PROJECT_DIR}/libraries/lodepng/
        ${PROJECT_DIR}/libraries/External/dotnet/
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)

target_link_directories(engine PUBLIC ${PROJECT_DIR}/libraries/ ${PROJECT_DIR}/libraries/External/dotnet/)

target_link_libraries(engine PUBLIC ${OPENGL_LIBRARIES} $<TARGET_OBJECTS:glew> ${LIBRARY} ${PHYSX_LIBRARIES} cuda dl libnethost.so)