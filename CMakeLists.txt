cmake_minimum_required(VERSION 3.3)
project(opengl)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Including GLFW
if(EXISTS glfw)
    add_subdirectory(glfw)
    include_directories(glfw/include)
endif()

# Including GLAD
include_directories(glad/include)

set(SOURCE_FILES main.cpp glad/src/glad.c)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Linking GLFW and OGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
