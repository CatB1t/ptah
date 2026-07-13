cmake_minimum_required(VERSION 3.21)
project(ptah LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BUILD_SHARED_LIBS OFF)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_LIBRARY_TYPE STATIC)
include(FetchContent)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include>)
target_link_libraries(glad PUBLIC OpenGL::GL)
add_library(deps::glad ALIAS glad)

add_executable(ptah
    src/main.cpp
    src/context.cpp
    src/window.cpp
    src/renderer.cpp
    src/mesh.cpp
    src/shader.cpp
    src/utils/file_loading.cpp
)

target_include_directories(ptah PRIVATE include)
target_link_libraries(ptah PRIVATE
    deps::glad
    OpenGL::GL
    glfw
)
target_compile_definitions(ptah
PRIVATE
GLFW_INCLUDE_NONE
PTAH_SHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)
