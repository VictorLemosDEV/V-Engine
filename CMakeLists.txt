cmake_minimum_required(VERSION 3.28)
project(Vector_Learn)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLAD/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyobjloader
)

# --- AQUI ESTÁ A CORREÇÃO PRINCIPAL ---
# Check if we're using MSVC or MinGW and set the correct library directory
if(MSVC)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/lib-vc2022)
else()
    # For MinGW
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/lib-mingw-w64)
endif()

add_executable(Vector_Learn
        src/main.cpp
        src/glad.c
        src/algebra.hpp
        src/Shader.cpp
        src/Shader.hpp
        src/Mesh.hpp
        src/Mesh.cpp
        src/Texture.hpp
        src/Texture.cpp
        src/Camera.hpp
        src/Camera.cpp
        src/Model.hpp
        src/Model.cpp
)

# Link libraries
target_link_libraries(Vector_Learn PRIVATE opengl32 glfw3)

# Copy the GLFW DLL from the correct location based on compiler
if(MSVC)
    add_custom_command(TARGET Vector_Learn POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/lib-vc2022/glfw3.dll"
            "$<TARGET_FILE_DIR:Vector_Learn>"
    )
else()
    add_custom_command(TARGET Vector_Learn POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/libs/GLFW/lib-mingw-w64/glfw3.dll"
            "$<TARGET_FILE_DIR:Vector_Learn>"
    )
endif()

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_BINARY_DIR})