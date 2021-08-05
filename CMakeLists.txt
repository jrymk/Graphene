cmake_minimum_required(VERSION 3.15)
include(GenerateExportHeader)
include(FetchContent)
project(Graphene)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DNDEBUG -DMTR_ENABLED)


#FetchContent_Declare(
#        tracy
#        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
#        GIT_TAG master
#        GIT_SHALLOW TRUE
#        GIT_PROGRESS TRUE
#)
#FetchContent_MakeAvailable(tracy)

include_directories(cmake-build-debug/_deps/tracy-src)
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
include_directories(Dependencies/stduuid/include)
include_directories(Dependencies/stduuid/gsl)
include_directories(Dependencies/thread_pool)
include_directories(Dependencies/json)
include_directories(Dependencies/binn/src)
include_directories(Dependencies/ImGui-Addons/FileBrowser)
include_directories(Dependencies/earcut)
include_directories(Dependencies/minitrace)
include_directories(Sources/)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(Graphene
        #        cmake-build-debug/_deps/tracy-src/TracyClient.cpp
        Dependencies/glad/src/glad.c
        Dependencies/imgui/imgui_demo.cpp
        Dependencies/imgui/imgui_draw.cpp
        Dependencies/imgui/imgui_tables.cpp
        Dependencies/imgui/imgui_widgets.cpp
        Dependencies/imgui/imgui.cpp
        Dependencies/imgui/backends/imgui_impl_glfw.cpp
        Dependencies/imgui/backends/imgui_impl_opengl3.cpp
        Dependencies/binn/src/binn.c
        Dependencies/ImGui-Addons/FileBrowser/ImGuiFileBrowser.cpp
        Dependencies/minitrace/minitrace.c
        Sources/Objects/Uuid.cpp
        Sources/Properties/Properties.cpp
        Sources/Properties/PropertiesParser.cpp
        Sources/Graph/Graph.cpp
        Sources/Structure/Component.cpp
        Sources/Structure/Componentifier.cpp
        Sources/Main.cpp
        Sources/Properties/EdgeProps.cpp
        Sources/Properties/VertexProps.cpp
        Sources/Objects/Command.cpp
        Sources/Objects/Serializable.cpp
        Sources/Objects/Vec2.cpp
        Sources/Preferences/KeyBinding/KeyNames.cpp
        Sources/Preferences/KeyBinding/HotKey.cpp
        Sources/Preferences/KeyBinding/Bindings.cpp
        Sources/Preferences/KeyBinding/Actions.cpp
        Sources/Preferences/KeyBinding/Actions.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/glfw-3.3.4)
set(CMAKE_EXE_LINKER_FLAGS " -static")

target_link_libraries(Graphene
        #        PUBLIC Tracy::TracyClient
        imm32.lib
        glfw
        -static-libgcc
        -static-libstdc++
        )

target_compile_features(Graphene PRIVATE cxx_std_20)
