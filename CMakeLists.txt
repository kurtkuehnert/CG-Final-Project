cmake_minimum_required(VERSION 3.20)
project(Solar-System)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_executable(Main
    src/main.cpp
    src/menu.cpp
    src/texture.cpp
    src/input.cpp
    src/light.cpp
    src/window.cpp
    src/assets.cpp
    src/car.cpp
    src/object.cpp
    src/camera.cpp
    src/vector.cpp)

target_link_libraries(Main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
