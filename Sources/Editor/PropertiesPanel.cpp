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
            std::unordered_map<gfn::Uuid, gfn::VertexProps*> vertexProps;
            std::unordered_map<gfn::Uuid, gfn::EdgeProps*> edgeProps;
            if (!(getDoc(activeDoc)->graphview.selection.vertexSelection.empty() &&
                  getDoc(activeDoc)->graphview.selection.edgeSelection.empty())) {
                for (auto& s : getDoc(activeDoc)->graphview.selection.vertexSelection) {
                    if (props.getVertexProps(s))
                        vertexProps.insert({s, props.getVertexProps(s)});
                }
                for (auto& s : getDoc(activeDoc)->graphview.selection.edgeSelection) {
                    if (props.getEdgeProps(s))
                        edgeProps.insert({s, props.getEdgeProps(s)});
                }
            }
            static bool useRectPicker = false;

            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
            if (ImGui::CollapsingHeader("Vertex##PROPSPANEL")) {
                if (vertexProps.empty())
                    ImGui::Text("No vertices selected");
                else if (ImGui::BeginTable("##PROPSPANEL_VERTEXPROPSSPLIT", 2, ImGuiTableFlags_BordersH | ImGuiTableFlags_Resizable)) {
                    // uuid
                    /*{
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue90d UUID");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        if (vertexProps.size() == 1)
                            ImGui::Text(props.getVertexProps(vertexProps.begin()->first)->uuid.value.c_str());
                        else {
                            ImGui::PushDisabled(true);
                            ImGui::Text("multiple values");
                            ImGui::PopDisabled();
                        }
                    }*/
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
                        ImGui::PopItemWidth();
                        //if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                        double sx = 0, sy = 0;
                        for (auto& p : vertexProps) {
                            sx += p.second->position.value.x;
                            sy += p.second->position.value.y;
                        }
                        //}
                        widgetX = float(sx / vertexProps.size());
                        widgetY = float(sy / vertexProps.size());
                        widgetPrevX = widgetX;
                        widgetPrevY = widgetY;
                    }
                    // label
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue264 Label");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static char widget[1024];
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                            widget[0] = '\0';

                        if (vertexProps.size() == 1) {
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                                strcpy(widget, vertexProps.begin()->second->label.value.c_str());
                        } else {
                            ImGui::PushDisabled(true);
                            ImGui::PushFont(gfx.fontSmall);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.0f);
                            gfn::text("*multiple values", HUE_RED);
                            ImGui::PopFont();
                            ImGui::PopDisabled();
                        }
                        if (ImGui::InputTextMultiline("##PROPSPANEL_VERTEXLABEL", widget, 1024, ImVec2(ImGui::GetContentRegionAvailWidth(), 50.0f), 0) ||
                            ImGui::IsItemEdited()) {
                            for (auto& s : vertexProps) {
                                getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=label -value=\"" + std::string(widget) + "\"");
                            }
                        }
                        ImGui::PopItemWidth();
                    }
                    // label size
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue245 Label size");
                        ImGui::TableSetColumnIndex(1);
                        static bool widgetAuto;
                        static float widget = 0.5f;
                        ImGui::Checkbox("\ue65f Auto", &widgetAuto);
                        if (ImGui::IsItemEdited()) {
                            if (widgetAuto) {
                                for (auto& s : vertexProps)
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=labelSize -value=-1");
                            } else {
                                for (auto& s : vertexProps)
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=labelSize -value=" + std::to_string(widget));
                            }
                        }
                        if (!widgetAuto) {
                            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                            ImGui::DragFloat("##PROPSPANEL_VERTEXLABELSIZE", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                                double r = vertexProps.begin()->second->labelSize.value;
                                if (r >= 0)
                                    widget = float(r);
                                else if (r < 0)
                                    widgetAuto = true;
                            }
                            if (ImGui::IsItemEdited()) {
                                for (auto& s : vertexProps)
                                    getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=labelSize -value=" + std::to_string(widget));
                            }
                            ImGui::PopItemWidth();
                        }
                    }
                    // label color
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue23c Label color");
                        ImGui::TableSetColumnIndex(1);
                        static ImVec4 widget(0.0f, 0.0f, 0.0f, 1.0f);
                        bool edit = false;

                        static std::vector<uint32_t> colors;
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            colors.clear();
                            for (auto& p : vertexProps) {
                                bool unique = true;
                                for (auto& vc : colors) {
                                    if (vc == p.second->labelColor.value)
                                        unique = false;
                                }
                                if (unique)
                                    colors.push_back(p.second->labelColor.value);
                            }
                        }

                        /// PICKER
                        static std::vector<uint32_t> history;

                        if (ImGui::ColorButton("##PROPSPANEL_VERTEXLABELCOLOR", widget, ImGuiColorEditFlags_AlphaPreview))
                            ImGui::OpenPopup("##PROPSPANEL_VERTEXLABELCOLOR_POPUP");
                        /*ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 2.0f, ImGui::GetCursorScreenPos().y - 25.0f),
                                                            (widget.x + widget.y + widget.z < 1.5) ?
                                                            IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255),
                                                            "\ue145", nullptr);*/
                        if (ImGui::BeginPopup("##PROPSPANEL_VERTEXLABELCOLOR_POPUP")) {
                            ImGui::PushItemWidth(200.0f);
                            ImGui::ColorButton("##PROPSPANEL_VERTEXLABELCOLOR_PREVIEW", widget, ImGuiColorEditFlags_AlphaPreview, ImVec2(200.0f, 30.0f));

                            if (useRectPicker && gfn::button("hue bar##PROPSPANEL_VERTEXLABELCOLOR", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
                                useRectPicker = false;
                            if (!useRectPicker && gfn::button("hue ring##PROPSPANEL_VERTEXLABELCOLOR", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
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
                        if (ImGui::TreeNode("Selection##PROPSPANEL_VERTEXLABELCOLOR_SELECTION")) {
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
                                if (ImGui::ColorButton(("##PROPSPANEL_VERTEXLABELCOLOR_SELECTION_BTN" + std::to_string(count)).c_str(), rgba,
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
                        if (ImGui::TreeNode("Recents##PROPSPANEL_VERTEXLABELCOLOR_SELECTION")) {
                            if (!history.empty()) {
                                /*ImGui::PushDisabled(true);
                                ImGui::PushFont(gfx.smallDefault);
                                ImGui::Text("Recent colors");
                                ImGui::PopFont();
                                ImGui::PopDisabled();*/

                                int count = 0;
                                float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                                for (auto& c : history) {
                                    if (ImGui::ColorButton(("##PROPSPANEL_VERTEXLABELCOLOR_HISTORY" + std::to_string(count)).c_str(),
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
                                getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=labelColor -value=d" +
                                                           std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                            }
                        }
                    }
                    // enabled
                    /*{
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue53b Enabled");
                        ImGui::TableSetColumnIndex(1);
                        int mask = 0;
                        for (auto& p : vertexProps)
                            mask |= (p.second->enabled.value ? 2 : 1);
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
                    }*/
                    // vertexFillColor
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue23a Fill color");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                        bool edit = false;

                        static std::vector<uint32_t> colors;
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            colors.clear();
                            for (auto& p : vertexProps) {
                                bool unique = true;
                                for (auto& vc : colors) {
                                    if (vc == p.second->fillColor.value)
                                        unique = false;
                                }
                                if (unique)
                                    colors.push_back(p.second->fillColor.value);
                            }
                        }

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
                        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
                        if (ImGui::TreeNode("Selection##fillColor")) {
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
                                getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=fillColor -value=d" +
                                                           std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                            }
                        }
                        ImGui::PopItemWidth();
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
                            double r = vertexProps.begin()->second->radius.value;
                            widget = float(r);
                        }
                        if (ImGui::IsItemEdited()) {
                            for (auto& s : vertexProps)
                                getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=radius -value=" + std::to_string(widget));
                        }
                        ImGui::PopItemWidth();
                    }
                    // outline color
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue22b Outline color");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                        bool edit = false;

                        static std::vector<uint32_t> colors;
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            colors.clear();
                            for (auto& p : vertexProps) {
                                bool unique = true;
                                for (auto& vc : colors) {
                                    if (vc == p.second->fillColor.value)
                                        unique = false;
                                }
                                if (unique)
                                    colors.push_back(p.second->fillColor.value);
                            }
                        }

                        /// PICKER
                        static std::vector<uint32_t> history;

                        if (ImGui::ColorButton("##outlineColorSET", widget, ImGuiColorEditFlags_AlphaPreview))
                            ImGui::OpenPopup("##outlineColorPopupSET");
                        /*ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 2.0f, ImGui::GetCursorScreenPos().y - 25.0f),
                                                            (widget.x + widget.y + widget.z < 1.5) ?
                                                            IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255),
                                                            "\ue145", nullptr);*/
                        if (ImGui::BeginPopup("##outlineColorPopupSET")) {
                            ImGui::PushItemWidth(200.0f);
                            ImGui::ColorButton("##outlineColor2", widget, ImGuiColorEditFlags_AlphaPreview, ImVec2(200.0f, 30.0f));

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
                        if (ImGui::TreeNode("Selection##outlineColor")) {
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
                                if (ImGui::ColorButton(("##outlineColor" + std::to_string(count)).c_str(), rgba,
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
                                    if (ImGui::ColorButton(("##outlineColorHistory" + std::to_string(count)).c_str(),
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
                                getDoc(activeDoc)->execute("setvertexprops -uuid=" + s.first + " -key=outlineColor -value=d" +
                                                           std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                            }
                        }
                        ImGui::PopItemWidth();
                    }
                    // outline thickness
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue91a Outline thickness");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static float widget;
                        ImGui::DragFloat("##outlineThickness", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            double r = vertexProps.begin()->second->outlineThickness.value;
                            widget = float(r);
                        }
                        if (ImGui::IsItemEdited()) {
                            for (auto& s : vertexProps)
                                getDoc(activeDoc)->execute(
                                        "setvertexprops -uuid=" + s.first + " -key=outlineThickness -value=" + std::to_string(widget));
                        }
                        ImGui::PopItemWidth();
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
                            mask |= (p.second->pauseUpdate.value ? 2 : 1);
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
                        ImGui::PopItemWidth();
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
            if (ImGui::CollapsingHeader("Edge##PROPSPANEL")) {
                if (edgeProps.empty())
                    ImGui::Text("No edges selected");
                else if (ImGui::BeginTable("##PROPSPANEL_VERTEXPROPSSPLIT", 2, ImGuiTableFlags_BordersH | ImGuiTableFlags_Resizable)) {
                    // edge uuid
                    /*{
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue90d UUID");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        if (edgeProps.size() == 1)
                            ImGui::Text(props.getEdgeProps(edgeProps.begin()->first)->edgeUuid.value.c_str());
                        else {
                            ImGui::PushDisabled(true);
                            ImGui::Text("multiple values");
                            ImGui::PopDisabled();
                        }
                    }
                    // starting vertex
                    {
                        ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 60.0f);
                        gfn::button("u##PROPSPANEL_EDGEV_BUTTON", HUE_CONTRAST, HUE_CONTRAST, false, 30.0f, 0, false);

                        if (edgeProps.size() == 1)
                            ImGui::Text(props.getEdgeProps(edgeProps.begin()->first)->startVertexUuid.value.c_str());
                        else {
                            ImGui::PushDisabled(true);
                            ImGui::Text("multiple values");
                            ImGui::PopDisabled();
                        }
                    }
                    // ending vertex
                    {
                        ImGui::SameLine();
                        gfn::button("v##PROPSPANEL_EDGEV_BUTTON", HUE_CONTRAST, HUE_CONTRAST, false, 30.0f, 0, false);

                        if (edgeProps.size() == 1)
                            ImGui::Text(props.getEdgeProps(edgeProps.begin()->first)->endVertexUuid.value.c_str());
                        else {
                            ImGui::PushDisabled(true);
                            ImGui::Text("multiple values");
                            ImGui::PopDisabled();
                        }
                    }*/
                    // label
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue264 Label");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static char widget[1024];
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                            widget[0] = '\0';

                        if (edgeProps.size() == 1) {
                            if (getDoc(activeDoc)->graphview.selection.onChangeSelection)
                                strcpy(widget, edgeProps.begin()->second->label.value.c_str());
                        } else {
                            ImGui::PushDisabled(true);
                            ImGui::PushFont(gfx.fontSmall);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.0f);
                            gfn::text("*multiple values", HUE_RED);
                            ImGui::PopFont();
                            ImGui::PopDisabled();
                        }
                        if (ImGui::InputTextMultiline("##PROPSPANEL_EDGELABEL", widget, 1024, ImVec2(ImGui::GetContentRegionAvailWidth(), 50.0f), 0) ||
                            ImGui::IsItemEdited()) {
                            for (auto& s : edgeProps) {
                                getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=label -value=\"" + std::string(widget) + "\"");
                            }
                        }
                        ImGui::PopItemWidth();
                    }
                    // label size
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue245 Label size");
                        ImGui::TableSetColumnIndex(1);
                        static float widget = 0.5f;
                        if (ImGui::IsItemEdited()) {
                            for (auto& s : edgeProps)
                                getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=labelSize -value=" + std::to_string(widget));
                        }
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        ImGui::DragFloat("##PROPSPANEL_EDGELABELSIZE", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            double r = edgeProps.begin()->second->labelSize.value;
                            widget = float(r);
                        }
                        if (ImGui::IsItemEdited()) {
                            for (auto& s : edgeProps)
                                getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=labelSize -value=" + std::to_string(widget));
                        }
                        ImGui::PopItemWidth();
                    }
                    // label color
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue23c Label color");
                        ImGui::TableSetColumnIndex(1);
                        static ImVec4 widget(0.0f, 0.0f, 0.0f, 1.0f);
                        bool edit = false;

                        static std::vector<uint32_t> colors;
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            colors.clear();
                            for (auto& p : edgeProps) {
                                bool unique = true;
                                for (auto& vc : colors) {
                                    if (vc == p.second->labelColor.value)
                                        unique = false;
                                }
                                if (unique)
                                    colors.push_back(p.second->labelColor.value);
                            }
                        }

                        /// PICKER
                        static std::vector<uint32_t> history;

                        if (ImGui::ColorButton("##PROPSPANEL_EDGELABELCOLOR", widget, ImGuiColorEditFlags_AlphaPreview))
                            ImGui::OpenPopup("##PROPSPANEL_EDGELABELCOLOR_POPUP");
                        /*ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 2.0f, ImGui::GetCursorScreenPos().y - 25.0f),
                                                            (widget.x + widget.y + widget.z < 1.5) ?
                                                            IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255),
                                                            "\ue145", nullptr);*/
                        if (ImGui::BeginPopup("##PROPSPANEL_EDGELABELCOLOR_POPUP")) {
                            ImGui::PushItemWidth(200.0f);
                            ImGui::ColorButton("##PROPSPANEL_EDGELABELCOLOR_PREVIEW", widget, ImGuiColorEditFlags_AlphaPreview, ImVec2(200.0f, 30.0f));

                            if (useRectPicker && gfn::button("hue bar##PROPSPANEL_EDGELABELCOLOR", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
                                useRectPicker = false;
                            if (!useRectPicker && gfn::button("hue ring##PROPSPANEL_EDGELABELCOLOR", HUE_BLUE, HUE_TRANSPARENT, false, 0, 0, true))
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
                        if (ImGui::TreeNode("Selection##PROPSPANEL_EDGELABELCOLOR_SELECTION")) {
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
                                if (ImGui::ColorButton(("##PROPSPANEL_EDGELABELCOLOR_SELECTION_BTN" + std::to_string(count)).c_str(), rgba,
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
                        if (ImGui::TreeNode("Recents##PROPSPANEL_EDGELABELCOLOR_SELECTION")) {
                            if (!history.empty()) {
                                /*ImGui::PushDisabled(true);
                                ImGui::PushFont(gfx.smallDefault);
                                ImGui::Text("Recent colors");
                                ImGui::PopFont();
                                ImGui::PopDisabled();*/

                                int count = 0;
                                float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

                                for (auto& c : history) {
                                    if (ImGui::ColorButton(("##PROPSPANEL_EDGELABELCOLOR_HISTORY" + std::to_string(count)).c_str(),
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
                                getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=labelColor -value=d" +
                                                           std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                            }
                        }
                    }
                    // edge color
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\ue22b Edge color");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static ImVec4 widget(0.5f, 0.5f, 0.5f, 1.0f);
                        bool edit = false;

                        static std::vector<uint32_t> colors;
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            colors.clear();
                            for (auto& p : edgeProps) {
                                bool unique = true;
                                for (auto& vc : colors) {
                                    if (vc == p.second->edgeColor.value)
                                        unique = false;
                                }
                                if (unique)
                                    colors.push_back(p.second->edgeColor.value);
                            }
                        }

                        /// PICKER
                        static std::vector<uint32_t> history;

                        if (ImGui::ColorButton("##edgeColorSET", widget, ImGuiColorEditFlags_AlphaPreview))
                            ImGui::OpenPopup("##edgeColorPopupSET");
                        /*ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetCursorScreenPos().x + 2.0f, ImGui::GetCursorScreenPos().y - 25.0f),
                                                            (widget.x + widget.y + widget.z < 1.5) ?
                                                            IM_COL32(255, 255, 255, 255) : IM_COL32(0, 0, 0, 255),
                                                            "\ue145", nullptr);*/
                        if (ImGui::BeginPopup("##edgeColorPopupSET")) {
                            ImGui::PushItemWidth(200.0f);
                            ImGui::ColorButton("##edgeColor2", widget, ImGuiColorEditFlags_AlphaPreview, ImVec2(200.0f, 30.0f));

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
                        if (ImGui::TreeNode("Selection##edgeColor")) {
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
                                if (ImGui::ColorButton(("##edgeColor" + std::to_string(count)).c_str(), rgba,
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
                                    if (ImGui::ColorButton(("##edgeColorHistory" + std::to_string(count)).c_str(),
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
                                getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=edgeColor -value=d" +
                                                           std::to_string(ImGui::ColorConvertFloat4ToU32(widget)));
                            }
                        }
                        ImGui::PopItemWidth();
                    }
                    // arrow style
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("\uea18 Arrows");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                        static int widget = 0b0;
                        bool update = false;

                        if (gfn::button("\uf1e6##arrowcontrol", widget & 0b10 ? HUE_CONTRAST : HUE_DEFAULT, widget & 0b10 ? HUE_CONTRAST : HUE_DEFAULT, 0,
                                        (ImGui::GetContentRegionAvailWidth() - ImGui::GetStyle().ItemSpacing.x) / 2, 0, false)) {
                            if (widget & 0b10) widget &= ~0b10;
                            else widget |= 0b10;
                            update = true;
                        }

                        ImGui::SameLine();
                        if (gfn::button("\uf1df##arrowcontrol", widget & 0b1 ? HUE_CONTRAST : HUE_DEFAULT, widget & 0b1 ? HUE_CONTRAST : HUE_DEFAULT, 0,
                                        ImGui::GetContentRegionAvailWidth(), 0, false)) {
                            if (widget & 0b1) widget &= ~0b1;
                            else widget |= 0b1;
                            update = true;
                        }

                        if (update) {
                            for (auto& s : edgeProps)
                                getDoc(activeDoc)->execute("setedgeprops -uuid=" + s.first + " -key=arrowStyle -value=" + std::to_string(widget));
                        }

                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            widget = edgeProps.begin()->second->arrowStyle.value;
                        }
                        ImGui::PopItemWidth();
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
                        ImGui::DragFloat("##thickness", (float*) &widget, 0.01, FLT_MIN, FLT_MAX, "%.6f", 0);
                        if (getDoc(activeDoc)->graphview.selection.onChangeSelection) {
                            double r = edgeProps.begin()->second->thickness.value;
                            widget = float(r);
                        }
                        if (ImGui::IsItemEdited()) {
                            for (auto& s : edgeProps)
                                getDoc(activeDoc)->execute(
                                        "setedgeprops -uuid=" + s.first + " -key=thickness -value=" + std::to_string(widget));
                        }
                        ImGui::PopItemWidth();
                    }

                    ImGui::EndTable();
                }
            }
        } else
            ImGui::Text("No document selected");

        ImGui::PopStyleVar(1);

        ImGui::End();
    }
}

