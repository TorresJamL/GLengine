cmake_minimum_required(VERSION 3.26.0)
project(GLENGINE VERSION 1.0.0)
cmake_policy(SET CMP0027 NEW)

set(CMAKE_C_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_CXX_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
# find_package(GLFW3 REQUIRED)
add_subdirectory(dependencies/GLFW)


add_executable(GLENGINE
    src/main.cpp
    src/glad.c
    src/ElementBufferObj.cpp
    src/Shader.cpp
    src/VertexArrayObj.cpp
    src/VertexBufferObj.cpp
)

target_include_directories(GLENGINE 
    PRIVATE 
        dependencies
)

target_link_libraries(GLENGINE ${GLFW3_LIBRARY})    