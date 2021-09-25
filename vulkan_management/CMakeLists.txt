
cmake_minimum_required(VERSION 3.0)

project(
	vulkan_management
)

file(
	GLOB
	src_file
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.md
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_library(
	${PROJECT_NAME}
	${src_file}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	$<$<STREQUAL:${Vulkan_FOUND},TRUE>:${Vulkan_INCLUDE_DIRS}>
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	$<$<STREQUAL:${Vulkan_FOUND},TRUE>:${Vulkan_LIBRARIES}>
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
	_shader_entry_point_="main"
	$<$<CONFIG:Debug>:_debug_report_>
	$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>,_x86_arch_,_x64_arch_>
	$<$<CXX_COMPILER_ID:MSVC>:_platform_extension_="VK_KHR_win32_surface">
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	FOLDER
	"Vulkan"
)