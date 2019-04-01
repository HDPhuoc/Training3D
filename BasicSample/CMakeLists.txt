cmake_minimum_required(VERSION 2.8)
project( BasicSample )

find_package(OpenGL REQUIRED)

set(MY_DEPENDS "D:/Dependencies")

set(GLFW_DIR "${MY_DEPENDS}/glfw-3.2.1/install")
include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/lib)

set(GLEW_DIR "${MY_DEPENDS}/glew-2.1.0/install")
include_directories(${GLEW_DIR}/include)
add_definitions(-DGLEW_STATIC)
link_directories(${GLEW_DIR}/lib)

include_directories("${MY_DEPENDS}/glm-0.9.8.5")

include_directories("${MY_DEPENDS}/soil/src")
link_directories(${MY_DEPENDS}/soil/lib)


add_executable( BasicSample TestMain.cpp )
target_link_libraries(BasicSample ${OPENGL_LIBRARY} glfw3 libglew32d SOIL)