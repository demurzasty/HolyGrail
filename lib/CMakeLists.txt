cmake_minimum_required (VERSION 3.8.2)

set (BUILD_SHARED_LIBS OFF)

add_subdirectory ("glew-2.1.0")

set (GLFW_BUILD_EXAMPLES OFF)
set (GLFW_BUILD_TESTS OFF)
set (GLFW_BUILD_DOCS OFF)
set (GLFW_INSTALL OFF)

add_subdirectory ("glfw-3.3.8")

add_subdirectory ("stb")
