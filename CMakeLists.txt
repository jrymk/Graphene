cmake_minimum_required(VERSION 3.15)
include(GenerateExportHeader)
include(FetchContent)
project(Graphene)

set(CMAKE_CXX_STANDARD 20)

#add_definitions(-DNDEBUG)

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
include_directories(Dependencies/ImGuiFileDialog)
include_directories(Dependencies/ThreadPool)
include_directories(Dependencies/stb)
include_directories(Dependencies/earcut)
include_directories(Dependencies/simple-svg)
include_directories(Dependencies/lunasvg/include)
include_directories(Sources/)


find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(Graphene
        Dependencies/glad/src/glad.c
        Dependencies/imgui/imgui_demo.cpp
        Dependencies/imgui/imgui_draw.cpp
        Dependencies/imgui/imgui_tables.cpp
        Dependencies/imgui/imgui_widgets.cpp
        Dependencies/imgui/imgui.cpp
        Dependencies/imgui/backends/imgui_impl_glfw.cpp
        Dependencies/imgui/backends/imgui_impl_opengl3.cpp
        Dependencies/binn/src/binn.c
        Dependencies/ImGuiFileDialog/ImGuiFileDialog.cpp
        Sources/Core/Objects/Uuid.cpp
        Sources/Core/Properties/Properties.cpp
        Sources/Core/Properties/PropertiesParser.cpp
        Sources/Core/Graph/Graph.cpp
        Sources/Core/Structure/Component.cpp
        Sources/Core/Structure/Componentifier.cpp
        Sources/Main.cpp
        Sources/Core/Properties/EdgeProps.cpp
        Sources/Core/Properties/VertexProps.cpp
        Sources/Core/Objects/Args.cpp
        Sources/Core/Objects/Serializable.cpp
        Sources/Core/Objects/Vec2.cpp
        Sources/Editor/Preferences/KeyBindings/KeyNames.cpp
        Sources/Editor/Preferences/KeyBindings/HotKey.cpp
        Sources/Editor/Preferences/KeyBindings/Bindings.cpp
        Sources/Editor/Preferences/KeyBindings/Actions.cpp
        Sources/Core/Interface/TripleBuffer.tpp
        Sources/Editor/Preferences/Preferences.cpp
        Sources/Editor/HotKeyHandler.cpp
        Sources/Editor/Document/Document.cpp
        Sources/Editor/Editor.cpp
        Sources/Editor/Console.cpp
        Sources/Editor/DocsMgr.cpp
        Sources/Core/Objects/Timer.cpp
        Sources/Core/Placement/WithinComponent.cpp
        Sources/Core/Core/Core.cpp
        Sources/Core/Placement/Placement.cpp
        Sources/Core/Core/File.cpp
        Sources/Core/Core/Parser.cpp
        Sources/Core/Objects/Random.cpp
        Sources/Core/Core/Background.cpp
        Sources/Core/Interface/Interface.cpp
        Sources/Editor/Graphics/Graphics.cpp
        Sources/Core/Core/Meta.h
        Sources/Editor/Document/GraphView/Camera.cpp
        Sources/Editor/Document/GraphView/Selection.cpp
        Sources/Editor/Document/GraphView/GraphView.cpp
        Sources/Editor/Theme/Theme.cpp
        Sources/Editor/Theme/Styles.cpp
        Sources/Editor/Graphics/Barlow.cpp
        Sources/Editor/Graphics/Material.cpp
        Sources/Editor/Graphics/Arial.cpp
        Sources/Editor/Preferences/KeyBindings/KeyBindDialog.cpp
        Sources/Editor/PropertiesPanel.cpp
        Sources/Editor/Document/Export/ExportTikZ.cpp
        Sources/Editor/Document/Export/ExportMenu.cpp
        Sources/Editor/Document/Export/ExportSVG.cpp
        Sources/Editor/Document/Export/ExportPNG.cpp
        Sources/Core/Core/Parser.cpp
        Sources/Core/Objects/Random.cpp
        Sources/Core/Core/Background.cpp Sources/Core/Interface/Interface.cpp Sources/Editor/Graphics/Graphics.cpp Sources/Core/Core/Meta.h Sources/Editor/Document/GraphView/Camera.cpp
        Sources/Core/Structure/BlockCutTree/BlockCutTree.cpp
        Sources/Core/Structure/BlockCutTree/BlockCutTreeBuilder.cpp
        Sources/Core/Placement/PosInitializer.cpp
        )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Dependencies/glfw-3.3.4)

add_subdirectory(Dependencies/lunasvg)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)

set(CMAKE_EXE_LINKER_FLAGS " -static")

target_link_libraries(Graphene
        imm32.lib
        glfw
        lunasvg
        -static-libgcc
        -static-libstdc++
        )

target_compile_features(Graphene PRIVATE cxx_std_20)
