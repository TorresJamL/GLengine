cmake_minimum_required(VERSION 3.26.0)
project(GLENGINE VERSION 1.0.0)
cmake_policy(SET CMP0027 NEW)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
add_subdirectory(dependencies/GLFW)
add_subdirectory(dependencies/glm)

add_executable(GLENGINE
    J_engine/J_app/main.cpp
    J_engine/J_renderer/src/ElementBuffer.cpp
    J_engine/J_renderer/src/Shader.cpp
    J_engine/J_renderer/src/Texture.cpp
    J_engine/J_renderer/src/VertexArray.cpp
    J_engine/J_renderer/src/VertexBuffer.cpp
    J_engine/J_renderer/src/stb_image.cpp
)

target_link_libraries(GLENGINE PRIVATE glfw glm::glm OpenGL::GL)

target_include_directories(GLENGINE 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}J_engine/J_renderer/includes
        ${CMAKE_SOURCE_DIR}J_engine/J_renderer/resources
        ${CMAKE_SOURCE_DIR}/dependencies/glad/include
        ${CMAKE_SOURCE_DIR}/dependencies/glad/include/glad
)

target_sources(GLENGINE PRIVATE
    dependencies/glad/src/glad.c
)