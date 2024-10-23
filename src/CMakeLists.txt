if(MSVC)
  set(EXECUTABLE ${PROJECT_NAME})
else()
  set(EXECUTABLE ${PROJECT_NAME}.xc)
endif()

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(${EXECUTABLE} ${SOURCES})

target_include_directories(${EXECUTABLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXECUTABLE} ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES})

# Copy shaders to the build directory
add_custom_target(
  target_shaders
  COMMENT "Copying shaders"
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_dependencies(${EXECUTABLE} target_shaders)
