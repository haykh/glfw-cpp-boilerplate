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

set(BUILD_EXAMPLES
    OFF
    CACHE BOOL "Build examples")

find_package(glfw3 REQUIRED)
link_libraries(glfw)

add_subdirectory(src)
