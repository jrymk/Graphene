cmake_minimum_required(VERSION 3.15)
project(Graphene)

set(CMAKE_CXX_STANDARD 14)

include_directories(Dependencies/glad/include)
include_directories(Dependencies/glad/include/glad)
include_directories(Dependencies/glad/include/KHR)
include_directories(Dependencies/glfw-3.3.4.bin.WIN64)
include_directories(Dependencies/glfw-3.3.4.bin.WIN64/include)
include_directories(Dependencies/glfw-3.3.4.bin.WIN64/include/GLFW)
include_directories(Dependencies/imgui)
include_directories(Dependencies/imgui/backends)
include_directories(Dependencies/imgui/misc/cpp)
include_directories(Dependencies/imgui/misc/freetype)
include_directories(Dependencies/imgui/misc/single_file)
include_directories(Sources/)


find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

add_executable(Graphene
        Dependencies/glad/src/glad.c
        Dependencies/imgui/imgui_demo.cpp
        Dependencies/imgui/imgui_draw.cpp
        Dependencies/imgui/imgui_tables.cpp
        Dependencies/imgui/imgui_widgets.cpp
        Dependencies/imgui/imgui.cpp
        Dependencies/imgui/backends/imgui_impl_glfw.cpp
        Dependencies/imgui/backends/imgui_impl_opengl3.cpp
        Sources/Main.cpp)


target_link_libraries(Graphene imm32.lib)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/glfw-3.3.4)
set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(Graphene glfw)
target_link_libraries(Graphene -static-libgcc -static-libstdc++)

