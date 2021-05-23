#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../graphene/Include.h"
#include "Window.hpp"
#include "RawInput.hpp"
#include "../utils/Log.hpp"

namespace Gui {
    namespace LogWindow {

        void show() {

            ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin("Log");



            ImGui::End();
        }

    }

}

