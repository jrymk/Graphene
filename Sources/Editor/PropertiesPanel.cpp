#include "Editor.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace gfn {
    void Editor::showPropertiesPanel() {
        ImGui::Begin("\ue8b8 Properties");
        if (getDoc(activeDoc)) {
            auto props = getDoc(activeDoc)->getItf()->graph.getRead()->props;
            if (!(getDoc(activeDoc)->graphview.selection.vertexSelection.empty() &&
                  getDoc(activeDoc)->graphview.selection.edgeSelection.empty())) {
                std::unordered_map<gfn::Uuid, gfn::VertexProps*> vertexProps;
                for (auto& s : getDoc(activeDoc)->graphview.selection.vertexSelection) {
                    if (!props.getVertexProps(s))
                        assert(0);
                    vertexProps.insert({s, props.getVertexProps(s)});
                }
                std::unordered_map<gfn::Uuid, gfn::EdgeProps*> edgeProps;
                for (auto& s : getDoc(activeDoc)->graphview.selection.edgeSelection) {
                    if (!props.getEdgeProps(s))
                        assert(0);
                    edgeProps.insert({s, props.getEdgeProps(s)});
                }
                if (ImGui::BeginTabBar("PropsTabBar", 0)) {

                    if (ImGui::BeginTabItem("Vertex") && !vertexProps.empty()) {
                        // uuid
                        {
                            if (vertexProps.size() == 1)
                                ImGui::Text(props.getVertexProps(vertexProps.begin()->first)->uuid.get().c_str());
                        }
                        // enabled
                        {
                            int mask = 0;
                            for (auto& p : vertexProps)
                                mask |= (p.second->enabled.get() ? 2 : 1);
                            if (mask == 3)
                                ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                            bool checkbox = (mask == 2);
                            ImGui::Checkbox("\ue53b Enabled", &checkbox);
                            if (mask == 3)
                                ImGui::PopItemFlag();
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : vertexProps) {
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=enabled -value="
                                                               + (mask == 2 ? "false" : "true"));
                                }
                            }
                        }
                        ImGui::Separator();
                        // position
                        {
                            static float widget[2];
                            static float widgetPrev[2];
                            ImGui::DragFloat2("\ue89f Position", (float*) &widget, 1.0, -FLT_MAX, FLT_MAX, "%.6f", 0);
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : vertexProps) {
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=position -value=+(" +
                                                               std::to_string(widget[0] - widgetPrev[0]) + "," + std::to_string(widget[1] - widgetPrev[1]) +
                                                               ")");
                                }
                            }
                            //if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            double sx = 0, sy = 0;
                            for (auto& p : vertexProps) {
                                sx += p.second->position.get().x;
                                sy += p.second->position.get().y;
                            }
                            //}
                            widget[0] = float(sx / vertexProps.size());
                            widget[1] = float(sy / vertexProps.size());
                            widgetPrev[0] = widget[0];
                            widgetPrev[1] = widget[1];
                        }
                        ImGui::Separator();
                        // vertexFillColor
                        {
                            static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                            ImGui::PushItemWidth(200.0f);
                            ImGui::ColorPicker3("\ue3b7 Vertex fill color", (float*) &widget,
                                                ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel);
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                ImU32 c = vertexProps.begin()->second->vertexFillColor.get();
                                bool mixed = false;
                                for (auto& p : vertexProps) {
                                    if (p.second->vertexFillColor.get() != c)
                                        mixed = true;
                                }
                                if (!mixed)
                                    widget = ImGui::ColorConvertU32ToFloat4(c);
                            }
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : vertexProps) {
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=vertexFillColor -value=d" +
                                                               std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                                }
                            }
                        }
                        ImGui::Separator();
                        // radius
                        {
                            static float widget;
                            ImGui::DragFloat("\ue837 Radius", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                double r = vertexProps.begin()->second->radius.get();
                                bool mixed = false;
                                for (auto& p : vertexProps) {
                                    if (p.second->radius.get() != r)
                                        mixed = true;
                                }
                                if (!mixed)
                                    widget = float(r);
                            }
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : vertexProps)
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=radius -value=" + std::to_string(widget));
                            }
                        }
                        ImGui::Separator();
                        // lock (pause update)
                        {
                            int mask = 0;
                            for (auto& p : vertexProps)
                                mask |= (p.second->pauseUpdate.get() ? 2 : 1);
                            if (mask == 3)
                                ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                            bool checkbox = (mask == 2);
                            ImGui::Checkbox("\ue88d Lock", &checkbox);
                            if (mask == 3)
                                ImGui::PopItemFlag();
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : vertexProps) {
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=pauseUpdate -value="
                                                               + (mask == 2 ? "false" : "true"));
                                }
                            }
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Edge") && !edgeProps.empty()) {
                        // uuid
                        {
                            if (edgeProps.size() == 1) {
                                ImGui::Text(("Edge: " + props.getEdgeProps(edgeProps.begin()->first)->edgeUuid.get()).c_str());
                                ImGui::Text(("Start vertex: " + props.getEdgeProps(edgeProps.begin()->first)->startVertexUuid.get()).c_str());
                                ImGui::Text(("End vertex: " + props.getEdgeProps(edgeProps.begin()->first)->endVertexUuid.get()).c_str());
                            }
                        }
                        // enabled
                        {
                            int mask = 0;
                            for (auto& p : edgeProps)
                                mask |= (p.second->enabled.get() ? 2 : 1);
                            if (mask == 3)
                                ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                            bool checkbox = (mask == 2);
                            ImGui::Checkbox("\ue53b Enabled", &checkbox);
                            if (mask == 3)
                                ImGui::PopItemFlag();
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : edgeProps) {
                                    getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=enabled -value="
                                                               + (mask == 2 ? "false" : "true"));
                                }
                            }
                        }
                        ImGui::Separator();
                        // position
                        {
                            static float widget[2];
                            static float widgetPrev[2];
                            ImGui::DragFloat2("\ue89f Position", (float*) &widget, 1.0, -FLT_MAX, FLT_MAX, "%.6f", 0);
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : edgeProps) {
                                    getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=position -value=+(" +
                                                               std::to_string(widget[0] - widgetPrev[0]) + "," + std::to_string(widget[1] - widgetPrev[1]) +
                                                               ")");
                                }
                            }
                            //if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            double sx = 0, sy = 0;
                            for (auto& p : edgeProps) {
                                sx += p.second->position.get().x;
                                sy += p.second->position.get().y;
                            }
                            //}
                            widget[0] = float(sx / edgeProps.size());
                            widget[1] = float(sy / edgeProps.size());
                            widgetPrev[0] = widget[0];
                            widgetPrev[1] = widget[1];
                        }
                        ImGui::Separator();
                        // edgeFillColor
                        {
                            static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                            ImGui::PushItemWidth(200.0f);
                            ImGui::ColorPicker3("\ue3b7 Edge fill color", (float*) &widget,
                                                ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel);
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                ImU32 c = edgeProps.begin()->second->edgeFillColor.get();
                                bool mixed = false;
                                for (auto& p : edgeProps) {
                                    if (p.second->edgeFillColor.get() != c)
                                        mixed = true;
                                }
                                if (!mixed)
                                    widget = ImGui::ColorConvertU32ToFloat4(c);
                            }
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : edgeProps) {
                                    getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=edgeFillColor -value=d" +
                                                               std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                                }
                            }
                        }
                        ImGui::Separator();
                        // thickness
                        {
                            static float widget;
                            ImGui::DragFloat("\ue947 Thickness", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                double r = edgeProps.begin()->second->thickness.get();
                                bool mixed = false;
                                for (auto& p : edgeProps) {
                                    if (p.second->thickness.get() != r)
                                        mixed = true;
                                }
                                if (!mixed)
                                    widget = float(r);
                            }
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : edgeProps)
                                    getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=thickness -value=" + std::to_string(widget));
                            }
                        }
                        ImGui::EndTabItem();
                    }

                    ImGui::EndTabBar();
                }
            } else
                ImGui::Text("No object selected");
        } else
            ImGui::Text("No document selected");

        ImGui::End();
    }
}

