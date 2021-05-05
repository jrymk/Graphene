#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../utils/ExceptionHandler.hpp"
#include "../graphene/Include.h"


namespace Gui {

	namespace Toolbar {

		bool enableLiveUpdate = true;

		void show(Graphene::Core* core) {

			ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));
			ImGui::Begin("Toolbar");

			ImGui::Checkbox("Enable live update (L)", &enableLiveUpdate);

			if (ImGui::IsKeyPressed('L', false))
				enableLiveUpdate = !enableLiveUpdate;

			ImGui::SameLine();

			if (ImGui::Button("Update graph (U)") || enableLiveUpdate || ImGui::IsKeyPressed('U', true))
				core->updatePos();

			if (ImGui::Button("Reset constants to default")) {
				core->m_c1 = 2;
				core->m_c2 = 1;
				core->m_c3 = 1;
				core->m_c4 = 0.1;
			}

			ImGui::SliderFloat("C1", &core->m_c1, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("[C1] * log(d / C2)");

			ImGui::SliderFloat("C2", &core->m_c2, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("C1 * log(d / [C2])");

			ImGui::SliderFloat("C3", &core->m_c3, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("[C3] / (d^2)");

			ImGui::SliderFloat("C4", &core->m_c4, 0.001f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("Force multiplier");

			ImGui::Checkbox(u8"Show demo window", &showDemoWindow);

			ImGui::End();
		}

	}

}