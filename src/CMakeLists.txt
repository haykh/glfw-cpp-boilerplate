set(EXECUTABLE ${PROJECT_NAME}.xc)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(${EXECUTABLE} ${SOURCES})

target_include_directories(${EXECUTABLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXECUTABLE} ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES})
