#pragma once

#include <unordered_map>
#include <Properties/Properties.hpp>
#include <Editor/Documents.hpp>
#include <Editor/PropertiesPanel/PropsControls.hpp>

namespace gfn::editor {
    extern gfn::document::Document* getActiveDocument();

    void showPropertiesPanel() {
        ImGui::Begin("Properties");
        if (getActiveDocument()) {
            auto props = getActiveDocument()->interface.properties.getRead();
            if (!getActiveDocument()->graphview.selection.vertexSelection.empty()) {
                std::unordered_map<gfn::Uuid, gfn::props::VertexProps*> targetProps;
                for (auto& s : getActiveDocument()->graphview.selection.vertexSelection)
                    targetProps.insert({s, props->getVertexProps(s)});

                // uuid
                {
                    if (targetProps.size() == 1)
                        ImGui::Text(props->getVertexProps(targetProps.begin()->first)->uuid.get().c_str());
                }
                // enabled
                {
                    int mask = 0;
                    for (auto& p : targetProps)
                        mask |= (p.second->enabled.get() ? 2 : 1);
                    if (mask == 3)
                        ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                    bool checkbox = (mask == 2);
                    ImGui::Checkbox("Enabled", &checkbox);
                    if (mask == 3)
                        ImGui::PopItemFlag();
                    if (ImGui::IsItemEdited()) {
                        for (auto& s : targetProps) {
                            getActiveDocument()->execute("setvertexprops -uuid=" + s.first + " -key=enabled -value="
                                                         + (mask == 2 ? "false" : "true"));
                        }
                    }
                }
                // position

                // vertexFillColor
                {
                    ImU32 c = targetProps.begin()->second->vertexFillColor.get();
                    bool eq = true;
                    for (auto& p : targetProps) {
                        if (p.second->vertexFillColor.get() != c) {
                            eq = false;
                            break;
                        }
                    }
                    auto cf = ImGui::ColorConvertU32ToFloat4(c);
                    if (!eq)
                        cf = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
                    ImGui::ColorPicker4("Vertex fill color", &cf.x);
                    if (ImGui::IsItemEdited()) {
                        for (auto& s : targetProps) {
                        }
                    }
                }

            } else
                ImGui::Text("No object selected");
        } else
            ImGui::Text("No document selected");

        ImGui::End();
    }
}