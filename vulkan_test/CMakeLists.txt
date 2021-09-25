
cmake_minimum_required(VERSION 3.0)

project(
	vulkan_test
)

set(
	CMAKE_CXX_STANDARD
	20
)

file(
	GLOB
	src_file
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_executable(
	${PROJECT_NAME}
	${src_file}
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	gtest_main
	vulkan_algorithm
)

add_test(
	${PROJECT_NAME}
	${PROJECT_NAME}
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	FOLDER
	"Vulkan"
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	VS_DEBUGGER_COMMAND_ARGUMENTS
	"some dummy debug command"
	VS_DEBUGGER_WORKING_DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/../workspace/"
	VS_DEBUGGER_ENVIRONMENT
	"PATH=%PATH%;${CMAKE_CURRENT_SOURCE_DIR}"
)