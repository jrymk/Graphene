#pragma once

#include "imgui.h"
#include "imgui_spectrum.h"

namespace Gui {

	static void setTheme() {
		ImGuiStyle* style = &ImGui::GetStyle();

		style->DisplaySafeAreaPadding.x = 8.0f;
		style->DisplaySafeAreaPadding.y = 8.0f;
		style->WindowRounding = 4.0f;
		style->WindowBorderSize = 1.0f;
		style->WindowMenuButtonPosition = -1;
		style->GrabMinSize = 5.0f;
		style->FrameBorderSize = 1.2f;
		style->ChildBorderSize = 1.2f;
		style->TabBorderSize = 1.0f;
		style->FrameRounding = 4.0f;
		style->GrabRounding = 4.0f;
		style->WindowPadding = {15.0f, 10.0f};
		style->FramePadding = {6.0f, 4.0f};
		style->ScrollbarSize = 10.0f;
		style->ItemSpacing = {5.0f, 5.0f};

		style->Colors[ImGuiCol_WindowBg].w = 1.0f;
		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_SliderGrabHovered] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE400);
		colors[ImGuiCol_SliderGrabActive] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::BLUE700);
		colors[ImGuiCol_SliderBg] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY500);
		colors[ImGuiCol_SliderBgHovered] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
		colors[ImGuiCol_SliderBgActive] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::GRAY600);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.97f, 0.97f, 0.97f, 0.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.20f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.40f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.92f, 0.92f, 0.92f, 0.0f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.89f, 0.89f, 0.89f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.3f);
		colors[ImGuiCol_BorderShadow] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::Static::NONE); // We don't want shadows. Ever.
		colors[ImGuiCol_Separator] = ImVec4(0.59f, 0.59f, 0.59f, 0.62f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.56f, 1.00f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.67f, 0.26f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.67f, 0.26f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.88f, 0.88f, 0.88f, 0.93f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		colors[ImGuiCol_TabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.97f, 0.97f, 0.97f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.67f, 0.98f, 0.46f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.98f, 0.89f, 0.78f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.64f, 0.63f, 0.57f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.74f, 0.73f, 0.68f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.67f, 0.98f, 0.95f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.67f, 0.98f, 0.80f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	}

}