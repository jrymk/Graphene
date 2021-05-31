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

			ImGui::SameLine();

			updateGraph = false;
			if (ImGui::Button("Update graph") || enableLiveUpdate)
				updateGraph = true;

			if (ImGui::Button("Reset constants to default")) {
				Constants::c1 = 2;
				Constants::c2 = 1;
				Constants::c3 = 1;
                Constants::c4 = 0.1;
                Constants::c5 = 0.1;
                Constants::c6 = 0.001;
			}

			ImGui::SliderFloat("C1 (Vertex repel factor)", &Constants::c1, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("[C1] * log(d / C2)");

			ImGui::SliderFloat("C2 (Vertex repel exponent)", &Constants::c2, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("C1 * log(d / [C2])");

			ImGui::SliderFloat("C3 (Edge minimum length)", &Constants::c3, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("[C3] / (d^2)");

			ImGui::SliderFloat("C4 (Force multiplier)", &Constants::c4, 0.001f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			Gui::HelpMarker("Force multiplier");

            ImGui::SliderFloat("C5 (Torque multiplier)", &Constants::c5, 0.001f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SameLine();
            Gui::HelpMarker("?");

            ImGui::SliderFloat("C6 (Max coeff)", &Constants::c6, 0.00001f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SameLine();
            Gui::HelpMarker("?");


            ImGui::Checkbox(u8"Show demo window", &showDemoWindow);

			ImGui::End();
		}

	}

}