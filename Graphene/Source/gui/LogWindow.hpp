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
        bool autoScroll = true;
        const ImU32 severityCol[5] = {
                IM_COL32(168, 48, 44, 255),
                IM_COL32(168, 129, 44, 255),
                IM_COL32(102, 168, 44, 255),
                IM_COL32(44, 151, 168, 255),
                IM_COL32(148, 148, 148, 255),
        };

        void show() {

            ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin("Log");

            for (auto &it : Utils::Log::logBuffer) {
                switch (it.severity) {
                    case Utils::Log::LogItem::Severity::ERR:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(168, 48, 44, 255));
                        break;
                    case Utils::Log::LogItem::Severity::WARNING:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(168, 129, 44, 255));
                        break;
                    case Utils::Log::LogItem::Severity::INFO:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(102, 168, 44, 255));
                        break;
                    case Utils::Log::LogItem::Severity::DEBUG:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(44, 151, 168, 255));
                        break;
                    case Utils::Log::LogItem::Severity::VERBOSE:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(148, 148, 148, 255));
                        break;
                }
                ImGui::Text("%s", it.message.c_str());
                ImGui::PopStyleColor(1);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::Text("%s", (it.file + ":" + std::to_string(it.line)).c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::End();
        }

    }

}

