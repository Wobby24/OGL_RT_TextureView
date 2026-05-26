cmake_minimum_required(VERSION 3.16)
project(OGL_RT_TextureView VERSION 1.0 LANGUAGES CXX C)
find_package(OpenGL REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
endif()

# Output binary location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project directories (based on current repo layout)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")  # keep forward slashes and keep on one line
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/inc")
#set(EXTERNAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/ext")

# GLAD as OBJECT library
add_library(glad_objects OBJECT "${SRC_DIR}/glad/glad.c")
set_property(TARGET glad_objects PROPERTY COMPILE_DEFINITIONS "GLAD_GL_IMPLEMENTATION")
target_include_directories(glad_objects PRIVATE "${INCLUDE_DIR}")

# List Source files
set(PROJECT_SOURCES
    "${SRC_DIR}/OGL_RT_TextureView.cpp"
    "${SRC_DIR}/window/GLFWRenderSurface.cpp"
    "${SRC_DIR}/window/GLFW_Window.cpp"
    "${SRC_DIR}/util/Shader.cpp"
    "${SRC_DIR}/util/Texture.cpp"
)

# External dependencies
include(FetchContent)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable tests" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable docs" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

add_library(OGL_RT_TextureView STATIC
    ${PROJECT_SOURCES}
)
add_library(OGL_RT_TextureView::OGL_RT_TextureView ALIAS OGL_RT_TextureView)

target_link_libraries(OGL_RT_TextureView
    PUBLIC
        OpenGL::GL
        glfw
    PRIVATE
        glad_objects
)

# Post-build copy: only valid after the target exists
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
 #   COMMAND ${CMAKE_COMMAND} -E copy_directory
  #  "${CMAKE_SOURCE_DIR}/rt_output"
   # "$<TARGET_FILE_DIR:${PROJECT_NAME}>/rt_output"
#)

# Includes
target_include_directories(OGL_RT_TextureView
    PUBLIC
        "${INCLUDE_DIR}"
)

file(GLOB_RECURSE PROJECT_HEADERS
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
)

# Debug macro
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
)
