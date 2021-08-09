#include "Editor.h"
#include <Editor/Theme/Theme.h>
#include <set>
#include <imgui.h>
#include <imgui_internal.h>

namespace gfn {
    void Editor::showPropertiesPanel() {
        ImGui::Begin("\ue8b8 Properties");
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(6.0f, 6.0f));
        if (getDoc(activeDoc)) {
            auto props = getDoc(activeDoc)->getItf()->graph.getRead()->props;
            if (!(getDoc(activeDoc)->graphview.selection.vertexSelection.empty() &&
                  getDoc(activeDoc)->graphview.selection.edgeSelection.empty())) {
                std::unordered_map<gfn::Uuid, gfn::VertexProps*> vertexProps;
                for (auto& s : getDoc(activeDoc)->graphview.selection.vertexSelection) {
                    if (props.getVertexProps(s))
                        vertexProps.insert({s, props.getVertexProps(s)});
                }
                std::unordered_map<gfn::Uuid, gfn::EdgeProps*> edgeProps;
                for (auto& s : getDoc(activeDoc)->graphview.selection.edgeSelection) {
                    if (props.getEdgeProps(s))
                        edgeProps.insert({s, props.getEdgeProps(s)});
                }
                if (ImGui::BeginTabBar("PropsTabBar", 0)) {
                    if (ImGui::BeginTabItem("Vertex")) {
                        if (vertexProps.empty())
                            ImGui::Text("No vertices selected");
                        else if (ImGui::BeginTable("vertexPropsSplit", 2, ImGuiTableFlags_BordersH | ImGuiTableFlags_Resizable)) {
                            // uuid
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue90d UUID");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                if (vertexProps.size() == 1)
                                    ImGui::Text(props.getVertexProps(vertexProps.begin()->first)->uuid.get().c_str());
                                else {
                                    ImGui::PushDisabled(true);
                                    ImGui::Text("multiple values");
                                    ImGui::PopDisabled();
                                }
                            }
                            // name
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue262 Label");
                                ImGui::TableSetColumnIndex(1);
                                static char widget[1024];
                                if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                                    widget[0] = '\0';

                                if (vertexProps.size() == 1) {
                                    if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                                        strcpy(widget, vertexProps.begin()->second->label.get().c_str());

                                    if (ImGui::InputText("", widget, 1024, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemEdited()) {
                                        getDoc(activeDoc)->execute("setvertexprops -uuid=" + vertexProps.begin()->first +
                                                                   " -key=label -value=" + std::string(widget));
                                    }
                                    ImGui::PopItemWidth();
                                } else {
                                    ImGui::PushDisabled(true);
                                    ImGui::Text("multiple values");
                                    ImGui::PopDisabled();
                                }
                            }
                            // enabled
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue53b Enabled");
                                ImGui::TableSetColumnIndex(1);
                                int mask = 0;
                                for (auto& p : vertexProps)
                                    mask |= (p.second->enabled.get() ? 2 : 1);
                                if (mask == 3)
                                    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                                bool checkbox = (mask == 2);
                                ImGui::Checkbox("##vertexEnabled", &checkbox);
                                if (mask == 3)
                                    ImGui::PopItemFlag();
                                if (ImGui::IsItemEdited()) {
                                    for (auto& s : vertexProps) {
                                        getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=enabled -value="
                                                                   + (mask == 2 ? "false" : "true"));
                                    }
                                }
                                ImGui::SameLine();
                                if (gfn::button("delete", HUE_RED, HUE_DEFAULT, false, 0, 0, false)) {
                                    for (auto& s : vertexProps) {
                                        getDoc(activeDoc)->execute("rmvertex -uuid=" + s.first);
                                    }
                                }
                            }
                            // position
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue89f Position");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() - ImGui::GetStyle().ItemSpacing.x) / 2);
                                static float widgetX;
                                static float widgetY;
                                static float widgetPrevX;
                                static float widgetPrevY;
                                bool edit = false;
                                ImGui::DragFloat("##vertexPositionX", &widgetX, 1.0, -FLT_MAX, FLT_MAX, "x: %.6f", 0);
                                if (ImGui::IsItemEdited()) edit = true;
                                ImGui::SameLine();
                                ImGui::DragFloat("##vertexPositionY", &widgetY, 1.0, -FLT_MAX, FLT_MAX, "y: %.6f", 0);
                                if (edit || ImGui::IsItemEdited()) {
                                    for (auto& s : vertexProps) {
                                        getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=position -value=+(" +
                                                                   std::to_string(widgetX - widgetPrevX) + "," +
                                                                   std::to_string(widgetY - widgetPrevY) + ")");
                                    }
                                }
                                //if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                double sx = 0, sy = 0;
                                for (auto& p : vertexProps) {
                                    sx += p.second->position.get().x;
                                    sy += p.second->position.get().y;
                                }
                                //}
                                widgetX = float(sx / vertexProps.size());
                                widgetY = float(sy / vertexProps.size());
                                widgetPrevX = widgetX;
                                widgetPrevY = widgetY;
                            }
                            // vertexFillColor
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue3b7 Vertex fill color");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                                static bool useRectPicker = false;
                                bool edit = false;

                                static std::vector<uint32_t> colors;
                                if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                    colors.clear();
                                    for (auto& p : vertexProps) {
                                        bool unique = true;
                                        for (auto& vc : colors) {
                                            if (vc == p.second->vertexFillColor.get())
                                                unique = false;
                                        }
                                        if (unique)
                                            colors.push_back(p.second->vertexFillColor.get());
                                    }
                                }

                                ImGui::PushItemWidth(200.0f);
                                /// PICKER
                                static std::vector<uint32_t> history;

                                if (ImGui::ColorButton("##vertexFillColorSET", widget, ImGuiColorEditFlags_AlphaPreview))
                                    ImGui::OpenPopup("##vertexFillColorPopupSET");
                                /*ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 2.0f, ImGui::GetCursorScreenPos().y - 25.0f),
                                                                    (widget.x + widget.y + widget.z < 1.5) ?
                                                                    IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255),
                                                                    "\ue145", nullptr);*/
                                if (ImGui::BeginPopup("##vertexFillColorPopupSET")) {
                                    ImGui::PushItemWidth(200.0f);
                                    ImGui::ColorButton("##vertexFillColor2", widget, ImGuiColorEditFlags_AlphaPreview, ImVec2(200.0f, 30.0f));

                                    if (useRectPicker && gfn::button("hue bar", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
                                        useRectPicker = false;
                                    if (!useRectPicker && gfn::button("hue ring", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
                                        useRectPicker = true;

                                    ImGui::ColorPicker3("", (float*) &widget,
                                                        ImGuiColorEditFlags_AlphaPreview |
                                                        (!useRectPicker * ImGuiColorEditFlags_PickerHueWheel) |
                                                        ImGuiColorEditFlags_NoSidePreview |
                                                        ImGuiColorEditFlags_NoSmallPreview);
                                    if (ImGui::IsItemEdited()) edit = true;
                                    ImGui::PopItemWidth();

                                    ImGui::EndPopup();
                                } else if (history.empty() || !history.empty() && history[0] != ImGui::ColorConvertFloat4ToU32(widget)) {
                                    if (std::find(history.begin(), history.end(), ImGui::ColorConvertFloat4ToU32(widget)) != history.end())
                                        history.erase(std::find(history.begin(), history.end(), ImGui::ColorConvertFloat4ToU32(widget)));
                                    history.insert(history.begin(), ImGui::ColorConvertFloat4ToU32(widget));
                                }

                                ImGui::SameLine();
                                ImGui::Button("\ue3b8");

                                float btnSize = 29.0f;
                                /// SELECTION PALATTE
                                if (ImGui::TreeNode("Selection")) {
                                    /*ImGui::PushDisabled(true);
                                    ImGui::PushFont(gfx.smallDefault);
                                    ImGui::Text("Pick a color from selection");
                                    ImGui::PopFont();
                                    ImGui::PopDisabled();*/

                                    std::vector<std::pair<float, std::pair<float, std::pair<float, float>>>> hsvV;
                                    for (auto& c : colors) {
                                        auto rgba = ImGui::ColorConvertU32ToFloat4(c);
                                        ImVec4 hsva = rgba;
                                        ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, hsva.x, hsva.y, hsva.z);
                                        hsvV.push_back({hsva.x, {hsva.y, {hsva.z, hsva.w}}});
                                    }
                                    std::sort(hsvV.begin(), hsvV.end());

                                    int count = 0;
                                    float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                                    for (auto& c : hsvV) {
                                        ImVec4 rgba;
                                        ImGui::ColorConvertHSVtoRGB(c.first, c.second.first, c.second.second.first, rgba.x, rgba.y, rgba.z);
                                        rgba.w = c.second.second.second;
                                        if (ImGui::ColorButton(("##vertexFillColor" + std::to_string(count)).c_str(), rgba,
                                                               ImGuiColorEditFlags_AlphaPreview)) {
                                            widget = rgba;
                                            edit = true;
                                        }
                                        float last_button_x2 = ImGui::GetItemRectMax().x;
                                        float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + btnSize;
                                        if (count + 1 < hsvV.size() && next_button_x2 < window_visible_x2)
                                            ImGui::SameLine();
                                        count++;
                                    }

                                    if (ImGui::IsItemEdited())
                                        edit = true;
                                    ImGui::TreePop();
                                }

                                /// RECENTS PALETTE
                                if (ImGui::TreeNode("Recents")) {
                                    if (!history.empty()) {
                                        /*ImGui::PushDisabled(true);
                                        ImGui::PushFont(gfx.smallDefault);
                                        ImGui::Text("Recent colors");
                                        ImGui::PopFont();
                                        ImGui::PopDisabled();*/

                                        int count = 0;
                                        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                                        for (auto& c : history) {
                                            if (ImGui::ColorButton(("##vertexFillColorHistory" + std::to_string(count)).c_str(),
                                                                   ImGui::ColorConvertU32ToFloat4(c),
                                                                   ImGuiColorEditFlags_AlphaPreview)) {
                                                widget = ImGui::ColorConvertU32ToFloat4(c);
                                                edit = true;
                                            }
                                            float last_button_x2 = ImGui::GetItemRectMax().x;
                                            float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + btnSize;
                                            if (count + 1 < history.size() && next_button_x2 < window_visible_x2)
                                                ImGui::SameLine();
                                            count++;
                                        }

                                        if (ImGui::IsItemEdited())
                                            edit = true;
                                    }
                                    ImGui::TreePop();
                                }
                                if (edit) {
                                    for (auto& s : vertexProps) {
                                        getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=vertexFillColor -value=d" +
                                                                   std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                                    }
                                }
                            }
                            // radius
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue837 Radius");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                static float widget;
                                ImGui::DragFloat("##vertexRadius", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
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
                            // lock (pause update)
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue88d Lock");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                int mask = 0;
                                for (auto& p : vertexProps)
                                    mask |= (p.second->pauseUpdate.get() ? 2 : 1);
                                if (mask == 3)
                                    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, true);
                                bool checkbox = (mask == 2);
                                ImGui::Checkbox("##vertexLock", &checkbox);
                                if (mask == 3)
                                    ImGui::PopItemFlag();
                                if (ImGui::IsItemEdited()) {
                                    for (auto& s : vertexProps) {
                                        getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=pauseUpdate -value="
                                                                   + (mask == 2 ? "false" : "true"));
                                    }
                                }
                            }
                            ImGui::EndTable();
                        }
                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem("Edge")) {
                        if (edgeProps.empty())
                            ImGui::Text("No edges selected");
                        else if (ImGui::BeginTable("edgePropsSplit", 2, ImGuiTableFlags_BordersH | ImGuiTableFlags_Resizable)) {
                            // edge uuid
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue90d UUID");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                if (edgeProps.size() == 1)
                                    ImGui::Text(props.getEdgeProps(edgeProps.begin()->first)->edgeUuid.get().c_str());
                                else {
                                    ImGui::PushDisabled(true);
                                    ImGui::Text("multiple values");
                                    ImGui::PopDisabled();
                                }
                            }
                            // starting vertex
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue40c Starting vertex");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                if (edgeProps.size() == 1)
                                    ImGui::Text(props.getEdgeProps(edgeProps.begin()->first)->startVertexUuid.get().c_str());
                                else {
                                    ImGui::PushDisabled(true);
                                    ImGui::Text("multiple values");
                                    ImGui::PopDisabled();
                                }
                            }
                            // ending vertex
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue40c Ending vertex");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                if (edgeProps.size() == 1)
                                    ImGui::Text(props.getEdgeProps(edgeProps.begin()->first)->endVertexUuid.get().c_str());
                                else {
                                    ImGui::PushDisabled(true);
                                    ImGui::Text("multiple values");
                                    ImGui::PopDisabled();
                                }
                            }
                            // name
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue262 Label");
                                ImGui::TableSetColumnIndex(1);
                                static char widget[1024];
                                if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                                    widget[0] = '\0';

                                if (edgeProps.size() == 1) {
                                    if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                                        strcpy(widget, edgeProps.begin()->second->label.get().c_str());

                                    if (ImGui::InputText("", widget, 1024, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemEdited()) {
                                        getDoc(activeDoc)->execute("setedgeprops -uuid=" + edgeProps.begin()->first +
                                                                   " -key=label -value=" + std::string(widget));
                                    }
                                    ImGui::PopItemWidth();
                                } else {
                                    ImGui::PushDisabled(true);
                                    ImGui::Text("multiple values");
                                    ImGui::PopDisabled();
                                }
                            }
                            // edgeFillColor
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue3b7 Edge fill color");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                                static bool useRectPicker = false;
                                bool edit = false;

                                static std::vector<uint32_t> colors;
                                if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                    colors.clear();
                                    for (auto& p : edgeProps) {
                                        bool unique = true;
                                        for (auto& vc : colors) {
                                            if (vc == p.second->edgeFillColor.get())
                                                unique = false;
                                        }
                                        if (unique)
                                            colors.push_back(p.second->edgeFillColor.get());
                                    }
                                }

                                ImGui::PushItemWidth(200.0f);
                                /// PICKER
                                static std::vector<uint32_t> history;

                                if (ImGui::ColorButton("##edgeFillColorSET", widget, ImGuiColorEditFlags_AlphaPreview))
                                    ImGui::OpenPopup("##edgeFillColorPopupSET");
                                /*ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 2.0f, ImGui::GetCursorScreenPos().y - 25.0f),
                                                                    (widget.x + widget.y + widget.z < 1.5) ?
                                                                    IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255),
                                                                    "\ue145", nullptr);*/
                                if (ImGui::BeginPopup("##edgeFillColorPopupSET")) {
                                    ImGui::PushItemWidth(200.0f);
                                    ImGui::ColorButton("##edgeFillColor2", widget, ImGuiColorEditFlags_AlphaPreview, ImVec2(200.0f, 30.0f));

                                    if (useRectPicker && gfn::button("hue bar", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
                                        useRectPicker = false;
                                    if (!useRectPicker && gfn::button("hue ring", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
                                        useRectPicker = true;

                                    ImGui::ColorPicker3("", (float*) &widget,
                                                        ImGuiColorEditFlags_AlphaPreview |
                                                        (!useRectPicker * ImGuiColorEditFlags_PickerHueWheel) |
                                                        ImGuiColorEditFlags_NoSidePreview |
                                                        ImGuiColorEditFlags_NoSmallPreview);
                                    if (ImGui::IsItemEdited()) edit = true;
                                    ImGui::PopItemWidth();

                                    ImGui::EndPopup();
                                } else if (history.empty() || !history.empty() && history[0] != ImGui::ColorConvertFloat4ToU32(widget)) {
                                    if (std::find(history.begin(), history.end(), ImGui::ColorConvertFloat4ToU32(widget)) != history.end())
                                        history.erase(std::find(history.begin(), history.end(), ImGui::ColorConvertFloat4ToU32(widget)));
                                    history.insert(history.begin(), ImGui::ColorConvertFloat4ToU32(widget));
                                }

                                ImGui::SameLine();
                                ImGui::Button("\ue3b8");

                                float btnSize = 29.0f;
                                /// SELECTION PALATTE
                                if (ImGui::TreeNode("Selection")) {
                                    /*ImGui::PushDisabled(true);
                                    ImGui::PushFont(gfx.smallDefault);
                                    ImGui::Text("Pick a color from selection");
                                    ImGui::PopFont();
                                    ImGui::PopDisabled();*/

                                    std::vector<std::pair<float, std::pair<float, std::pair<float, float>>>> hsvV;
                                    for (auto& c : colors) {
                                        auto rgba = ImGui::ColorConvertU32ToFloat4(c);
                                        ImVec4 hsva = rgba;
                                        ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, hsva.x, hsva.y, hsva.z);
                                        hsvV.push_back({hsva.x, {hsva.y, {hsva.z, hsva.w}}});
                                    }
                                    std::sort(hsvV.begin(), hsvV.end());

                                    int count = 0;
                                    float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                                    for (auto& c : hsvV) {
                                        ImVec4 rgba;
                                        ImGui::ColorConvertHSVtoRGB(c.first, c.second.first, c.second.second.first, rgba.x, rgba.y, rgba.z);
                                        rgba.w = c.second.second.second;
                                        if (ImGui::ColorButton(("##edgeFillColor" + std::to_string(count)).c_str(), rgba,
                                                               ImGuiColorEditFlags_AlphaPreview)) {
                                            widget = rgba;
                                            edit = true;
                                        }
                                        float last_button_x2 = ImGui::GetItemRectMax().x;
                                        float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + btnSize;
                                        if (count + 1 < hsvV.size() && next_button_x2 < window_visible_x2)
                                            ImGui::SameLine();
                                        count++;
                                    }

                                    if (ImGui::IsItemEdited())
                                        edit = true;
                                    ImGui::TreePop();
                                }

                                /// RECENTS PALETTE
                                if (ImGui::TreeNode("Recents")) {
                                    if (!history.empty()) {
                                        /*ImGui::PushDisabled(true);
                                        ImGui::PushFont(gfx.smallDefault);
                                        ImGui::Text("Recent colors");
                                        ImGui::PopFont();
                                        ImGui::PopDisabled();*/

                                        int count = 0;
                                        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                                        for (auto& c : history) {
                                            if (ImGui::ColorButton(("##edgeFillColorHistory" + std::to_string(count)).c_str(),
                                                                   ImGui::ColorConvertU32ToFloat4(c),
                                                                   ImGuiColorEditFlags_AlphaPreview)) {
                                                widget = ImGui::ColorConvertU32ToFloat4(c);
                                                edit = true;
                                            }
                                            float last_button_x2 = ImGui::GetItemRectMax().x;
                                            float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + btnSize;
                                            if (count + 1 < history.size() && next_button_x2 < window_visible_x2)
                                                ImGui::SameLine();
                                            count++;
                                        }

                                        if (ImGui::IsItemEdited())
                                            edit = true;
                                    }
                                    ImGui::TreePop();
                                }
                                if (edit) {
                                    for (auto& s : edgeProps) {
                                        getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=edgeFillColor -value=d" +
                                                                   std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                                    }
                                }
                            }
                            // thickness
                            {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("\ue91a Thickness");
                                ImGui::TableSetColumnIndex(1);
                                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                                static float widget;
                                ImGui::DragFloat("##edgethickness", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
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

                            ImGui::EndTable();
                        }
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
            } else
                ImGui::Text("No object selected");
        } else
            ImGui::Text("No document selected");

        ImGui::PopStyleVar(1);
        ImGui::End();
    }
}

