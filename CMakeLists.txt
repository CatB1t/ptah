cmake_minimum_required(VERSION 3.21)
project(ptah LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
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

FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_TESTS OFF)
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(assimp)

find_package(OpenGL REQUIRED)

add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include>)
target_link_libraries(glad PUBLIC OpenGL::GL)
add_library(deps::glad ALIAS glad)

add_library(stb STATIC external/stb/src/stb_image.c)
target_include_directories(stb PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/stb/include>)
add_library(deps::stb ALIAS stb)

add_executable(ptah
    demos/lights.cpp
    src/core/context.cpp
    src/core/window.cpp
    src/core/renderer.cpp
    src/core/mesh.cpp
    src/core/material.cpp
    src/core/material_instance.cpp
    src/core/material_props.cpp
    src/model.cpp
    src/orbit_camera.cpp
    src/core/data_buffer.cpp
    src/core/mirrored_buffer.cpp
    src/core/texture2d.cpp
    src/core/helpers/gizmos.cpp
    src/input.cpp
    src/utils/file_loading.cpp
    src/utils/logger.cpp
)

target_include_directories(ptah PRIVATE include)
target_link_libraries(ptah PRIVATE
    deps::glad
    deps::stb
    OpenGL::GL
    glfw
    spdlog::spdlog
    glm::glm
    assimp::assimp
)

target_compile_definitions(ptah
PRIVATE
GLFW_INCLUDE_NONE
PTAH_SHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
PTAH_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
PTAH_GIZMOS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/gizmos"
)
