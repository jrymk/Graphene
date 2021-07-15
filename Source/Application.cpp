#include <iostream>
#include <thread>
#include "editor/Include.h"
#include "core/Core.hpp"
//#include "editor/panels/graphview-legacy//GraphView/GraphView.hpp"
#include "editor/panels/graphview/GraphView.hpp"
#include "editor/panels/LogWindow-legacy.hpp"
#include "editor/graphic/DoubleBuffer.hpp"
#include "editor/graphic/Graphic.hpp"
#include "core/BlockCutTreeBuilder.hpp"
#include "core/Core.hpp"
#include "core/GraphicExporter.hpp"
#include "../../../Dependencies/imgui/imgui.h"

void updateGraphLoop(graphene::core::Core& core, bool& update, bool& end) {
	while (!end) {
		if (update)
			core.updatePos();
	}
}

int main() {
	graphene::core::Core core;

	for (int i = 0; i < 100; i++)
		core.getGraphObj()->newVertex();

	bool updateGraphAsync = true;
	// Multi-threading is temporarily disabled because it leads to crash
	bool endBackgroundUpdateLoop = false;
	std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(updateGraphAsync),
									   std::ref(endBackgroundUpdateLoop));
	backgroundUpdateThread.detach();

	graphene::editor::graphic::init();

	if (graphene::editor::frameworkInit(graphene::editor::frameworkConfig())) {
		while (graphene::editor::newFrame()) {
			// core.updatePos();

			graphene::editor::graphview::show();
			// graphene::GraphView::show(&core);
			ImGui::ShowDemoWindow();
			// Gui::LogWindow::show();
			ImGui::Begin("Graph View on rendering thread");
			// if (graphene::editor::graphic::mutex.try_lock()) {
			// graphene::editor::graphic::swapReady = false;
			if (graphene::editor::graphic::swapReady && graphene::editor::graphic::backBufferUpdated)
				graphene::editor::graphic::swap();

			graphene::editor::graphic::frontBuffer->writeReady = false;
			for (auto v : graphene::editor::graphic::frontBuffer->vertices) {
				ImGui::GetWindowDrawList()->AddCircleFilled(
					ImVec2(ImGui::GetCursorScreenPos().x + v.position.x * 200.0f - 100.0f,
						   ImGui::GetCursorScreenPos().y + v.position.y * 200.0f - 100.0f),
					5.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 20);
			}
			if (graphene::editor::graphic::frontBuffer->vertices.size() != 100)
				std::cerr << "OOPS! skipped a frame for some reason\n";
			else
				std::cerr << "good frame, good boy\n";

			graphene::editor::graphic::frontBuffer->writeReady = true;
			// graphene::editor::graphic::swapReady = true;
			// graphene::editor::graphic::mutex.unlock();

			// LOG_DEBUG("render thread swap buffer successful");
			//}
			ImGui::End();

			graphene::editor::endFrame();
		}
	}
	endBackgroundUpdateLoop = true;
}
