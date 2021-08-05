#pragma once

#include <imgui.h>

namespace gfn::theme {
    void setImGui() {
        ImGui::GetStyle().FrameRounding = 6.0f;
        ImGui::GetStyle().FrameBorderSize = 1.0f;
        ImGui::GetStyle().TabBorderSize = 1.0f;
        ImGui::GetStyle().TabBorderSize = 1.0f;
        ImGui::GetStyle().WindowPadding = ImVec2(16.0f, 16.0f);
        ImGui::GetStyle().FramePadding = ImVec2(4.0f, 3.0f);
        ImGui::GetStyle().ItemSpacing = ImVec2(8.0f, 6.0f);
        ImGui::GetStyle().GrabMinSize = 12.0f;
        ImGui::GetStyle().WindowRounding = 6.0f;
        ImGui::GetStyle().ChildRounding = 6.0f;
        ImGui::GetStyle().PopupRounding = 6.0f;
        ImGui::GetStyle().TabRounding = 6.0f;
        ImGui::GetStyle().GrabRounding = 6.0f;
        ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.96f, 0.96f, 0.96f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.97f, 0.97f, 0.97f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.77f, 0.77f, 0.77f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.54f, 0.82f, 1.00f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.72f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.60f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.51f, 0.84f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.30f, 0.67f, 0.91f, 0.41f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.74f, 0.90f, 1.00f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.81f, 0.92f, 1.00f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.30f, 0.68f, 0.94f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.30f, 0.68f, 0.94f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }
}