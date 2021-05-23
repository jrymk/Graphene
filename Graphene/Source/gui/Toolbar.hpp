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
		bool updateGraph = false;

		void show(Graphene::Core* core) {

			ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));
			ImGui::Begin("Toolbar");

			ImGui::Checkbox("Enable live update (L)", &enableLiveUpdate);

			if (ImGui::IsKeyPressed('L', false))
				enableLiveUpdate = !enableLiveUpdate;

			ImGui::SameLine();

			updateGraph = false;
			if (ImGui::Button("Update graph (U)") || enableLiveUpdate || ImGui::IsKeyPressed('U', true))
				updateGraph = true;

			if (ImGui::Button("Reset constants to default")) {
				core->c1 = 2;
				core->c2 = 1;
				core->c3 = 1;
				core->c4 = 0.1;
			}

			ImGui::SliderFloat("C1 (Vertex repel factor)", &core->c1, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("[C1] * log(d / C2)");

			ImGui::SliderFloat("C2 (Vertex repel exponent)", &core->c2, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("C1 * log(d / [C2])");

			ImGui::SliderFloat("C3 (Edge minimum length)", &core->c3, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("[C3] / (d^2)");

			ImGui::SliderFloat("C4 (Force multiplier)", &core->c4, 0.001f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("Force multiplier");

			ImGui::Checkbox(u8"Show demo window", &showDemoWindow);

			ImGui::End();
		}

	}

}