set(EXECUTABLE ${PROJECT_NAME}.xc)

file(GLOB_RECURSE SOURCES "*.cpp" "*.c")

add_executable(${EXECUTABLE} ${SOURCES})
target_include_directories(${EXECUTABLE} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
