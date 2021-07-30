#pragma once

#include <imgui.h>
#include <Interface/Interface.hpp>
#include <Preferences/Preferences.hpp>

#include <Document/GraphView/Camera.hpp>
#include <Document/GraphView/Renderer.hpp>
#include <Document/GraphView/Selection.hpp>

namespace gfn::editor::graphview {
///@brief renders and handles interaction to a specified interface
class GraphView {
  public:
	gfn::Uuid documentUuid;
	gfn::interface::Interface* interface = nullptr;
	gfn::preferences::Preferences* preferences = nullptr;

	Camera camera;
	Renderer renderer;
	Selection selection;

	GraphView() = default;

	void init(const gfn::Uuid& _documentUuid, gfn::interface::Interface* _interface,
			  gfn::preferences::Preferences* _preferences) {
		documentUuid = _documentUuid;
		interface = _interface;
		preferences = _preferences;

		renderer.interface = _interface;
		renderer.camera = &camera;

		selection.interface = _interface;
		selection.preferences = preferences;
		selection.camera = &camera;
	}

	void update() {
		assert(interface && preferences); // did you forget to bind interface or preferences?

		// ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
		// auto binding = preferences->getKeyBind().getBinding(gfn::keybind::ACTION_CAMERA_PAN);
		ImGui::InvisibleButton("graphview_canvas",
							   ImVec2(std::max(ImGui::GetContentRegionAvail().x, 10.0f),
									  std::max(ImGui::GetContentRegionAvail().y, 10.0f)),
							   ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

		camera.update(preferences);
		selection.update();

		if (!selection.hoveredVertex.empty()) {
			auto props = interface->userprops.getRead()->getVertexProps(selection.hoveredVertex);
			ImGui::GetWindowDrawList()->AddCircleFilled(camera.map(props->position),
														camera.map(props->radius + preferences->glow_size),
														IM_COL32(0, 255, 255, 255), 0);
		} else if (!selection.hoveredEdge.empty()) {
			auto edgeProps = interface->userprops.getRead()->getEdgeProps(selection.hoveredEdge);
			auto uProps = interface->userprops.getRead()->getVertexProps(edgeProps->startVertexUuid);
			auto vProps = interface->userprops.getRead()->getVertexProps(edgeProps->endVertexUuid);
			ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position), camera.map(vProps->position),
												IM_COL32(0, 255, 255, 255),
												camera.map(edgeProps->thickness + preferences->glow_size * 2.0));
		}

		if (!selection.leftMouseDownVertex.empty()) {
			auto uProps = interface->userprops.getRead()->getVertexProps(selection.leftMouseDownVertex);
			if (!selection.hoveredVertex.empty()) {
				auto vProps = interface->userprops.getRead()->getVertexProps(selection.hoveredVertex);
				ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position), camera.map(vProps->position),
													IM_COL32(0, 255, 0, 100), camera.map(0.2f));
			} else {
				ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position), ImGui::GetMousePos(),
													IM_COL32(0, 255, 0, 100), camera.map(0.2f));
			}
		}

		renderer.drawEdges();
		renderer.drawVertices();

		// ImGui::PopStyleVar(1);
	}
};
} // namespace gfn::editor::graphview