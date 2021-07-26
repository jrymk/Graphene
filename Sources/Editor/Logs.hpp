#pragma once

#include <imgui.h>
#include <Core/Logging/Logging.hpp>

#undef ERROR

namespace gfn::editor::logs {
bool autoScroll = true;
int filter = 1;

const ImU32 severityCol[5] = {
	IM_COL32(168, 48, 44, 255),	 IM_COL32(168, 129, 44, 255),  IM_COL32(102, 168, 44, 255),
	IM_COL32(44, 151, 168, 255), IM_COL32(148, 148, 148, 255),
};

void show(gfn::core::logging::LogBuffer* logBuffer) {
	// std::cerr << logBuffer->buffer.size() << "\n";
	//auto buffer = *logBuffer;
	bool open = true;
	bool goToBottom = false;
	ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));

	if (ImGui::Begin("Log", &open, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Options")) {
				if (ImGui::MenuItem("Clear all"))
                    logBuffer->buffer.clear();
				if (ImGui::MenuItem("Go to bottom"))
					goToBottom = true;
				ImGui::MenuItem("Auto scroll", nullptr, &autoScroll);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Filter")) {
				// I just want to make a radio button :(

				bool fVerbose = (filter == 0);
				bool fDebug = (filter == 1);
				bool fInfo = (filter == 2);
				bool fWarning = (filter == 3);
				bool fError = (filter == 4);

				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(148, 148, 148, 255));
				ImGui::MenuItem("Verbose", nullptr, &fVerbose);
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(44, 151, 168, 255));
				ImGui::MenuItem("Debug", nullptr, &fDebug);
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(102, 168, 44, 255));
				ImGui::MenuItem("Info", nullptr, &fInfo);
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(168, 129, 44, 255));
				ImGui::MenuItem("Warning", nullptr, &fWarning);
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(168, 48, 44, 255));
				ImGui::MenuItem("Error", nullptr, &fError);
				ImGui::PopStyleColor(5);

				int tempFilter = -1;
				if (fVerbose && filter != 0)
					tempFilter = 0;
				if (fDebug && filter != 1)
					tempFilter = 1;
				if (fInfo && filter != 2)
					tempFilter = 2;
				if (fWarning && filter != 3)
					tempFilter = 3;
				if (fError && filter != 4)
					tempFilter = 4;
				if (tempFilter != -1)
					filter = tempFilter;

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		for (auto& it : logBuffer->buffer) {
			int severity = 0;
			ImU32 textColorTemp = ImGui::GetColorU32(ImGuiCol_Text);
			switch (it->severity) {
			case gfn::core::logging::LogEntry::Severity::ERROR:
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(168, 48, 44, 255));
				severity = 4;
				break;
			case gfn::core::logging::LogEntry::Severity::WARNING:
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(168, 129, 44, 255));
				severity = 3;
				break;
			case gfn::core::logging::LogEntry::Severity::INFO:
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(102, 168, 44, 255));
				severity = 2;
				break;
			case gfn::core::logging::LogEntry::Severity::DEBUG:
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(44, 151, 168, 255));
				severity = 1;
				break;
			case gfn::core::logging::LogEntry::Severity::VERBOSE:
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(148, 148, 148, 255));
				severity = 0;
				break;
			default:
				ImGui::PushStyleColor(ImGuiCol_Text, textColorTemp);
			}

			if (severity < filter) {
				ImGui::PopStyleColor(1);
				continue;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
			ImGui::Text("%s", it->timeStamp.c_str());
			ImGui::SameLine();
			ImGui::SetCursorPosX(180.0f);
			ImGui::PopStyleColor(1);
			ImGui::Text("%s", it->message.c_str());
			ImGui::PopStyleColor(1);

			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("%s", (it->file + ":" + std::to_string(it->line)).c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		if ((autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) || goToBottom)
			ImGui::SetScrollHereY(1.0f);

		ImGui::End();
	}
}

} // namespace gfn::editor::logs