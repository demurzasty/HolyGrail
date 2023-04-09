cmake_minimum_required (VERSION 3.8.2)

project (TheHolyGrail)

add_subdirectory ("lib")

add_executable (TheHolyGrail 
    "src/Application.cpp"
    "src/Chunk.cpp"
    "src/Main.cpp"
    "src/Math.cpp"
    "src/Matrix4.cpp"
    "src/Shader.cpp"
    "src/Texture.cpp"
    "src/Vector2.cpp"
    "src/Vector3.cpp"
    "src/Vector4.cpp"
)
target_link_libraries (TheHolyGrail PUBLIC glew glfw stb)
target_compile_features (TheHolyGrail PUBLIC cxx_std_17)

if (MSVC) 
	target_compile_definitions (TheHolyGrail PUBLIC _CRT_SECURE_NO_WARNINGS)
endif ()

add_custom_command(
    TARGET TheHolyGrail POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            ${CMAKE_CURRENT_BINARY_DIR}/data
)
