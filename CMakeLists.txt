cmake_minimum_required(VERSION 3.26.0)
project(GLENGINE VERSION 1.0.0)
cmake_policy(SET CMP0027 NEW)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
add_subdirectory(dependencies/GLFW)
add_subdirectory(dependencies/glm)

add_executable(GLENGINE
    src/main.cpp
    src/ElementBuffer.cpp
    src/Shader.cpp
    src/VertexArray.cpp
    src/VertexBuffer.cpp
)

target_link_libraries(GLENGINE PRIVATE glfw OpenGL::GL)

target_include_directories(GLENGINE 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_SOURCE_DIR}/dependencies/glad/include
        ${CMAKE_SOURCE_DIR}/dependencies/glad/include/glad
)

target_sources(GLENGINE PRIVATE
    dependencies/glad/src/glad.c
)