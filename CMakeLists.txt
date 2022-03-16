cmake_minimum_required(VERSION 3.20)
project(Solar-System)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(Main
    help.cpp
    image.cpp
    input.cpp
    light.cpp
    wavefront.cpp
    window.cpp
    assets.cpp
    planet.cpp
    planetSimulation.cpp
    Main.cpp)

target_link_libraries(Main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARIES})
