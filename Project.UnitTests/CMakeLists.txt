file(GLOB_RECURSE TEST_SRC "src/*.cpp")

set(Boost_ADDITIONAL_VERSIONS "1.67")

find_package(Boost 1.67 REQUIRED COMPONENTS unit_test_framework)

enable_testing()

add_executable(unit_test ${TEST_SRC})
target_include_directories(unit_test PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${PROJECT_DIR}/Project.Engine/src/
    ${PROJECT_DIR}/Project.Game/src/
    ${PROJECT_DIR}/libraries/
    ${PROJECT_DIR}/libraries/physx/include/
    ${PROJECT_DIR}/libraries/lodepng/
    ${PROJECT_DIR}/Project.Game/src/
    ${PROJECT_DIR}/Project.UnitTests/src/
)
target_compile_definitions(unit_test PRIVATE "BOOST_TEST_DYN_LINK=1")
target_link_libraries(unit_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} engine)

add_test(NAME mytest COMMAND unit_test )