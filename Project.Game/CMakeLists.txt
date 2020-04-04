file(GLOB_RECURSE GAME_SRC "src/*.cpp")

add_executable(game ${GAME_SRC})

target_include_directories(game 
    PRIVATE
        ${PROJECT_DIR}/Project.Engine/src/
        ${PROJECT_DIR}/Project.Game/src/
        ${PROJECT_DIR}/libraries/
        ${PROJECT_DIR}/libraries/physx/include/
        ${PROJECT_DIR}/libraries/lodepng/
        ${PROJECT_DIR}/Project.Game/src/ 
)

target_link_libraries( game PUBLIC engine)

# file(COPY ${PROJECT_DIR}/res DESTINATION ${EXECUTABLE_OUTPUT_PATH})