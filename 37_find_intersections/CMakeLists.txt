cmake_minimum_required(VERSION 3.24)
project(find_intersections)

add_executable(${PROJECT_NAME} main.cpp)
//set(cinolib_DIR "/home/luca/Documenti/cinolib")
target_link_libraries(${PROJECT_NAME} cinolib)
set(CINOLIB_USES_OPENGL_GLFW_IMGUI ON)
find_package(cinolib REQUIRED)
