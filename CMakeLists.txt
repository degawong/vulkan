cmake_minimum_required(VERSION 3.0)

project(
	vulkan
)

set(CMAKE_CXX_STANDARD 20)

enable_testing(
)

find_package(
	Vulkan
	QUIET
)

set_property(
	GLOBAL
	PROPERTY
	USE_FOLDERS
	on
)

if(NOT ${Vulkan_FOUND})
	return()
endif(NOT ${Vulkan_FOUND})

set(
	BENCHMARK_ENABLE_INSTALL OFF
)

set(
	BENCHMARK_ENABLE_TESTING OFF
)

set(
	BENCHMARK_ENABLE_GTEST_TESTS OFF
)

set(
	BENCHMARK_ENABLE_ASSEMBLY_TESTS OFF
)

set(
	gtest_force_shared_crt ON CACHE BOOL "always use mdd" FORCE
)

add_subdirectory(
	benchmark
)

add_subdirectory(
	googletest
)
add_subdirectory(
	vulkan_management
)

add_subdirectory(
	vulkan_algorithm
)

add_subdirectory(
	vulkan_test
)

add_subdirectory(
	vulkan_benchmark
)