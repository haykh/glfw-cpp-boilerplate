cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "glfw_boilerplate")

project(
  ${PROJECT_NAME}
  VERSION 0.0.1
  DESCRIPTION "A simple boilerplate for GLFW"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEBUG
    OFF
    CACHE BOOL "Debug mode")

if(${DEBUG} STREQUAL "OFF")
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -DNDEBUG")
  message(STATUS "Release mode")
else()
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  message(STATUS "Debug mode")
endif()

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glfw3)

if(${glfw3_FOUND})
  message(STATUS "GLFW found")
else()
  message(STATUS "GLFW not found, using bundled version")
  set(BUILD_EXAMPLES
      OFF
      CACHE BOOL "")
  set(BUILD_SHARED_LIBS
      OFF
      CACHE BOOL "")
  set(GLFW_BUILD_EXAMPLES
      OFF
      CACHE BOOL "")
  set(GLFW_BUILD_TESTS
      OFF
      CACHE BOOL "")
  set(GLFW_BUILD_DOCS
      OFF
      CACHE BOOL "")
  set(GLFW_INSTALL
      OFF
      CACHE BOOL "")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/deps ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

add_subdirectory(src)
