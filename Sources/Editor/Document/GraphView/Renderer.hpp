#pragma once

#include <imgui.h>
#include <Core/Interface/Interface.h>
#include <Editor/Document/GraphView/Camera.h>
#include <Editor/Graphics/Graphics.h>
#include <Tracy.hpp>

namespace gfn {
    class Renderer {
    private:
        gfn::Interface* itf;
        Camera* camera;
        Selection* selection;

    public:
        Renderer(gfn::Interface* itf, Camera* camera, Selection* selection) :
                itf(itf),
                camera(camera),
                selection(selection) {

        }

        void drawEdges(gfn::Graphics* gfx) {
            ZoneScoped

            camera->xMin = FLT_MAX;
            camera->xMax = -FLT_MAX;
            camera->yMin = FLT_MAX;
            camera->yMax = -FLT_MAX;

            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = itf->graph.getRead()->props;
            for (auto& ei : userprops.getEdgePropsList()) {
                auto e = ei.second;
                if (!e.enabled.value)
                    continue;
                /*drawList->AddLine(camera->map(u), camera->map(v),
                                  e.edgeColor.value, camera->map(e.thickness.value));*/

                /*if (e.startVertexUuid.value == e.endVertexUuid.value) {

                    continue;
                }*/

                if (e.startVertexUuid.value == e.endVertexUuid.value) {
                    gfn::Vec2 vec = e.position.value - e.startVertexPosition.value;
                    drawList->AddBezierCubic(camera->map(e.startVertexPosition.value),
                                             camera->map(e.startVertexPosition.value + vec.rotate(M_PI_4)),
                                             camera->map(e.startVertexPosition.value + vec.rotate(-M_PI_4)),
                                             camera->map(e.endVertexPosition.value),
                                             e.edgeColor.value,
                                             camera->map(e.thickness.value));
                    continue;
                }

                float fixEdgeEndpoints = 0.0f;

                gfn::Vec2 u = e.startVertexPosition.value + (e.endVertexPosition.value - e.startVertexPosition.value).normalize() *
                                                            fixEdgeEndpoints * (userprops.getVertexProps(e.startVertexUuid.value)->radius.value +
                                                                                userprops.getVertexProps(e.startVertexUuid.value)->outlineThickness.value /
                                                                                2.0);
                gfn::Vec2 ep = e.position.value;
                double arrowCompensation = userprops.getVertexProps(e.endVertexUuid.value)->radius.value +
                                           userprops.getVertexProps(e.endVertexUuid.value)->outlineThickness.value / 2.0;
                gfn::Vec2 v =
                        u + (e.endVertexPosition.value - u).normalize() * ((e.endVertexPosition.value - u).length() - fixEdgeEndpoints * arrowCompensation);

                drawList->AddBezierQuadratic(camera->map(u + (v - u).normalize() * ((e.arrowStyle.value & 0b10) * 0.1 * (e.thickness.value / 0.06))),
                                             camera->map(ep),
                                             camera->map(u + (v - u).normalize() * ((v - u).length() -
                                                                                    (e.arrowStyle.value & 0b1) * 0.1 * (e.thickness.value / 0.06))),
                                             e.edgeColor.value,
                                             camera->map(e.thickness.value));
                /*drawList->AddBezierCurve(camera->map(u), camera->map(ep),
                                         camera->map(ep), camera->map(v), e.edgeColor.value,
                                         camera->map(e.thickness.value));*/

                static gfn::Vec2 arrowHead[] = {
                        {0.0,  0.0},
                        {-0.3, -0.13},
                        {-0.2, 0.0},
                        {-0.3, 0.13}
                };

                if (e.arrowStyle.value & 0b1) {
                    std::vector<ImVec2> transformedArrowHead;
                    double theta = acos((v.x - u.x) / (v - u).length()) * (v.y - u.y >= 0 ? -1.0 : 1.0);
                    for (auto& p : arrowHead)
                        transformedArrowHead.emplace_back(camera->map((p * (e.thickness.value / 0.06)).rotate(theta) + v));
                    drawList->AddTriangleFilled(transformedArrowHead[0], transformedArrowHead[2], transformedArrowHead[1], e.edgeColor.value);
                    drawList->AddTriangleFilled(transformedArrowHead[0], transformedArrowHead[2], transformedArrowHead[3], e.edgeColor.value);
                }
                if (e.arrowStyle.value & 0b10) {
                    std::vector<ImVec2> transformedArrowHead;
                    double theta = acos((u.x - v.x) / (u - v).length()) * (u.y - v.y >= 0 ? -1.0 : 1.0);
                    for (auto& p : arrowHead)
                        transformedArrowHead.emplace_back(camera->map((p * (e.thickness.value / 0.06)).rotate(theta) + u));
                    drawList->AddTriangleFilled(transformedArrowHead[0], transformedArrowHead[2], transformedArrowHead[1], e.edgeColor.value);
                    drawList->AddTriangleFilled(transformedArrowHead[0], transformedArrowHead[2], transformedArrowHead[3], e.edgeColor.value);
                }

                std::string label = ei.second.label.value;
                ImGui::PushFont(gfx->fontScalable);
                float scale = camera->zoom / 256.0f * e.labelSize.value;
                ImGui::SetWindowFontScale(scale);
                auto labelBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);

                gfn::Vec2 midpoint((u + v) / 2.0);
                gfn::Vec2 vector(v - u);
                gfn::Vec2 location(
                        midpoint + (vector.rotate(M_PI_2).normalize() *
                                    (camera->rMap(std::sqrt(labelBB.x * labelBB.x + labelBB.y * labelBB.y)) / 2.0 + e.thickness.value / 2.0)));

                drawList->AddText(ImVec2(camera->map(location).x - labelBB.x / 2.0f, camera->map(location).y - labelBB.y / 2.0f * 1.1f),
                                  e.labelColor.value, label.c_str());
                ImGui::PopFont();
                ImGui::SetWindowFontScale(1.0f);

//                drawList->AddCircleFilled(camera->map(ep), camera->map(e.thickness.value),
//                                          IM_COL32(255, 0, 0, 255), 0);

                camera->xMin = std::min(camera->xMin, ep.x);
                camera->xMax = std::max(camera->xMax, ep.x);
                camera->yMin = std::min(camera->yMin, ep.y);
                camera->yMax = std::max(camera->yMax, ep.y);
            }
        }

        void drawVertices(gfn::Graphics* gfx) {
            ZoneScoped

            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = itf->graph.getRead()->props;
            for (auto& vi : userprops.getVertexPropsList()) {
                auto v = vi.second;
                if (!v.enabled.value)
                    continue;

                drawList->AddCircleFilled(camera->map(v.position.value), camera->map(v.radius.value),
                                          v.fillColor.value, 0);
                drawList->AddCircle(camera->map(v.position.value), camera->map(v.radius.value),
                                    v.outlineColor.value, 0, camera->map(v.outlineThickness.value));

                std::string label = vi.second.label.value;
                ImGui::PushFont(gfx->fontScalable);
                float scale;
                if (v.labelSize.value < 0) {
                    auto fontBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);
                    scale = camera->zoom / 256.0f / std::sqrt(fontBB.x * fontBB.x + fontBB.y * fontBB.y) * 256.0f * 2.0 *
                            (v.radius.value - v.outlineThickness.value / 2.0f) * 0.9;
                } else {
                    scale = camera->zoom / 256.0f * v.labelSize.value;
                }
                ImGui::SetWindowFontScale(scale);
                auto labelBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);

                drawList->AddText(ImVec2(camera->map(v.position.value).x - labelBB.x / 2.0f, camera->map(v.position.value).y - labelBB.y / 2.0f * 1.1f),
                                  v.labelColor.value, label.c_str());
                ImGui::PopFont();
                ImGui::SetWindowFontScale(1.0f);

                camera->xMin = std::min(camera->xMin, v.position.value.x);
                camera->xMax = std::max(camera->xMax, v.position.value.x);
                camera->yMin = std::min(camera->yMin, v.position.value.y);
                camera->yMax = std::max(camera->yMax, v.position.value.y);
            }

            /*drawList->AddRect(camera->map(gfn::Vec2(camera->xMin, camera->yMin)), camera->map(gfn::Vec2(camera->xMax, camera->yMax)), IM_COL32(0, 255, 0, 255),
                              0, 0, 2.0f);*/
        }
    };
}