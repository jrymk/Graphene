#pragma once

#include <unordered_map>
#include <imgui.h>
#include <Properties/Properties.hpp>
#include <Editor/Documents.hpp>
#include <Editor/PropertiesPanel/PropsControls.hpp>

namespace gfn::editor {
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
                {
                    static float widget[2];
                    static float widgetPrev[2];
                    ImGui::DragFloat2("Position", (float*) &widget, 1.0, -FLT_MAX, FLT_MAX, "%.6f", 0);
                    if (ImGui::IsItemEdited()) {
                        for (auto& s : targetProps) {
                            getActiveDocument()->execute("setvertexprops -uuid=" + s.first + " -key=position -value=+(" +
                                                         std::to_string(widget[0] - widgetPrev[0]) + "," + std::to_string(widget[1] - widgetPrev[1]) + ")");
                        }
                    }
                    //if (getActiveDocument()->graphview.selection.onChangeSelection) {
                    double sx = 0, sy = 0;
                    for (auto& p : targetProps) {
                        sx += p.second->position.get().x;
                        sy += p.second->position.get().y;
                    }
                    //}
                    widget[0] = float(sx / targetProps.size());
                    widget[1] = float(sy / targetProps.size());
                    widgetPrev[0] = widget[0];
                    widgetPrev[1] = widget[1];
                }
                // vertexFillColor
                {
                    static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                    ImGui::PushItemWidth(200.0f);
                    ImGui::ColorPicker3("Vertex fill color", (float*) &widget,
                                        ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel);
                    if (getActiveDocument()->graphview.selection.onChangeSelection) {
                        ImU32 c = targetProps.begin()->second->vertexFillColor.get();
                        bool mixed = false;
                        for (auto& p : targetProps) {
                            if (p.second->vertexFillColor.get() != c)
                                mixed = true;
                        }
                        if (!mixed)
                            widget = ImGui::ColorConvertU32ToFloat4(c);
                    }
                    if (ImGui::IsItemEdited()) {
                        for (auto& s : targetProps) {
                            getActiveDocument()->execute("setvertexprops -uuid=" + s.first + " -key=vertexFillColor -value=d" +
                                                         std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                        }
                    }
                }
                // position
                {
                    static float widget;
                    ImGui::DragFloat("Radius", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                    if (getActiveDocument()->graphview.selection.onChangeSelection) {
                        double r = targetProps.begin()->second->radius.get();
                        bool mixed = false;
                        for (auto& p : targetProps) {
                            if (p.second->radius.get() != r)
                                mixed = true;
                        }
                        if (!mixed)
                            widget = float(r);
                    }
                    if (ImGui::IsItemEdited()) {
                        for (auto& s : targetProps)
                            getActiveDocument()->execute("setvertexprops -uuid=" + s.first + " -key=radius -value=" + std::to_string(widget));
                    }
                }

            } else
                ImGui::Text("No object selected");
        } else
            ImGui::Text("No document selected");

        ImGui::End();
    }
}