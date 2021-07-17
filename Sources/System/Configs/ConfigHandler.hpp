#pragma once

#define configAssert(x)                                                                                                \
	if (!x) {                                                                                                          \
		logInsert("Config: Read line error: ");                                                                        \
		logWarning(#x);                                                                                                \
	} else {                                                                                                           \
		logInsert("Config: Read line successful: ");                                                                   \
		logInfo(#x);                                                                                                   \
	}

#include <assert.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <imgui.h>
#include <imgui_internal.h>
#include <System/Logging/Logging.hpp>

#include <System/Window/WindowConfigs.hpp>

namespace gfn::config {

ImGuiSettingsHandler handler;
std::string typeName;
std::string config;

static void clearAllCallback(ImGuiContext* ctx, ImGuiSettingsHandler*) { logInfo("Config: Clear all"); }

static void applyAllCallback(ImGuiContext* ctx, ImGuiSettingsHandler*) { logInfo("Config: Apply all"); }

void* readOpenCallback(ImGuiContext*, ImGuiSettingsHandler*, const char* name) {
	logInfo("Config: Read open");
	config = name;
	logInsert("Config name: ");
	logInfo(config);
	return nullptr;
}

static void readLineCallback(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line) {
	logInfo("Config: Read line");

	if (config == "Window") {
		logInsert("Config name: ");
		logInfo(config);
		configAssert(sscanf(line, "Maximize=%d", &gfn::window::config::maximize) != 1);
		configAssert(sscanf(line, "DisplaySize=%d,%d", &gfn::window::config::width, &gfn::window::config::height) != 2);
		return;
	}
}

static void writeAllCallback(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf) {
	logInfo("Config: Write all");

	buf->reserve(1024);

	buf->appendf("[Graphene][Window]\n");
	buf->appendf("Maximize=%d\n", gfn::window::config::maximize);
	buf->appendf("DisplaySize=%d,%d\n", gfn::window::config::width, gfn::window::config::height);
	buf->append("\n");

	buf->append("\n");
}

void initConfigHandler() {
	ImGuiContext& g = *ImGui::GetCurrentContext();
	handler.TypeName = "Graphene";
	handler.TypeHash = ImHashStr("Graphene");
	handler.ClearAllFn = clearAllCallback;
	handler.ReadOpenFn = readOpenCallback;
	handler.ReadLineFn = readLineCallback;
	handler.ApplyAllFn = applyAllCallback;
	handler.WriteAllFn = writeAllCallback;
	g.SettingsHandlers.push_back(handler);
}
} // namespace gfn::config