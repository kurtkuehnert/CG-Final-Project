cmake_minimum_required(VERSION 3.20)
project(Solar-System)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(Main
    src/main.cpp
    src/help.cpp
    src/texture.cpp
    src/input.cpp
    src/light.cpp
    src/window.cpp
    src/assets.cpp
    src/object.cpp)

target_link_libraries(Main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARIES})
