cmake_minimum_required(VERSION 3.15)
project(Graphene)

set(CMAKE_CXX_STANDARD 14)


include_directories(Dependencies/freetype/builds/amiga/include)
include_directories(Dependencies/freetype/builds/amiga/include/config)
include_directories(Dependencies/freetype/builds/atari)
include_directories(Dependencies/freetype/builds/vms)
include_directories(Dependencies/freetype/devel)
include_directories(Dependencies/freetype/include)
include_directories(Dependencies/freetype/include/freetype)
include_directories(Dependencies/freetype/include/freetype/config)
include_directories(Dependencies/freetype/include/freetype/internal)
include_directories(Dependencies/freetype/include/freetype/internal/services)
include_directories(Dependencies/freetype/src/autofit)
include_directories(Dependencies/freetype/src/base)
include_directories(Dependencies/freetype/src/bdf)
include_directories(Dependencies/freetype/src/cache)
include_directories(Dependencies/freetype/src/cff)
include_directories(Dependencies/freetype/src/cid)
include_directories(Dependencies/freetype/src/gxvalid)
include_directories(Dependencies/freetype/src/gzip)
include_directories(Dependencies/freetype/src/lzw)
include_directories(Dependencies/freetype/src/otvalid)
include_directories(Dependencies/freetype/src/pcf)
include_directories(Dependencies/freetype/src/pfr)
include_directories(Dependencies/freetype/src/psaux)
include_directories(Dependencies/freetype/src/pshinter)
include_directories(Dependencies/freetype/src/psnames)
include_directories(Dependencies/freetype/src/raster)
include_directories(Dependencies/freetype/src/sdf)
include_directories(Dependencies/freetype/src/sfnt)
include_directories(Dependencies/freetype/src/smooth)
include_directories(Dependencies/freetype/src/truetype)
include_directories(Dependencies/freetype/src/type1)
include_directories(Dependencies/freetype/src/type42)
include_directories(Dependencies/freetype/src/winfonts)
include_directories(Dependencies/glad/include)
include_directories(Dependencies/glad/include/glad)
include_directories(Dependencies/glad/include/KHR)
include_directories(Dependencies/glfw-3.3.4.bin.WIN64)
include_directories(Dependencies/glfw-3.3.4.bin.WIN64/include)
include_directories(Dependencies/glfw-3.3.4.bin.WIN64/include/GLFW)
include_directories(Dependencies/glm-0.9.9.8/glm)
include_directories(Dependencies/glm-0.9.9.8/glm/detail)
include_directories(Dependencies/glm-0.9.9.8/glm/ext)
include_directories(Dependencies/glm-0.9.9.8/glm/gtc)
include_directories(Dependencies/glm-0.9.9.8/glm/gtx)
include_directories(Dependencies/glm-0.9.9.8/glm/simd)
include_directories(Dependencies/imgui)
include_directories(Dependencies/imgui/backends)
include_directories(Dependencies/imgui/examples/example_allegro5)
include_directories(Dependencies/imgui/examples/libs/gl3w/GL)
include_directories(Dependencies/imgui/examples/libs/glfw)
include_directories(Dependencies/imgui/examples/libs/glfw/include)
include_directories(Dependencies/imgui/examples/libs/glfw/include/GLFW)
include_directories(Dependencies/imgui/examples/libs/usynergy)
include_directories(Dependencies/imgui/misc/cpp)
include_directories(Dependencies/imgui/misc/freetype)
include_directories(Dependencies/imgui/misc/single_file)
include_directories(Graphene/Source/graphene)
include_directories(Graphene/Source/gui)
include_directories(Graphene/Source/utils)

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
        Graphene/Source/Application.cpp)


target_link_libraries(Graphene imm32.lib)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/glfw-3.3.4)
target_link_libraries(Graphene glfw)

