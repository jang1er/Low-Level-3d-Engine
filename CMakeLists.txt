cmake_minimum_required(VERSION 3.29)
project(Low_Level_3d_Engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Low_Level_3d_Engine main.cpp
        src/Build/GladBuild.cpp
        src/common/shader.cpp
        src/common/shader.hpp
        src/common/Textures.cpp
        src/common/Textures.hpp
        src/common/controls.cpp
        src/common/controls.hpp
)

target_include_directories(Low_Level_3d_Engine SYSTEM PRIVATE "vendor/glm" "vendor/glad" "vendor/glfw/include")
target_include_directories(Low_Level_3d_Engine PUBLIC "src")

add_subdirectory(vendor/glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(Low_Level_3d_Engine OpenGL::GL glfw)

