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
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = itf->graph.getRead()->props;
            for (auto& ei : userprops.getEdgePropsList()) {
                auto e = ei.second;
                if (!e.enabled.get())
                    continue;
                drawList->AddLine(camera->map(e.startVertexPosition.get()), camera->map(e.endVertexPosition.get()),
                                  e.edgeColor.get(), camera->map(e.thickness.get()));

                /*drawList->AddBezierQuadratic(camera->map(e.startVertexPosition), camera->map(e.position),
                                             camera->map(e.endVertexPosition), e.edgeFillColor.color32,
                                             camera->map(e.thickness));*/

                //drawList->AddCircleFilled(camera->map(e.position), camera->map(e.radius), e.edgeNodeColor.color32, 0);

                std::string label = ei.second.label.get();
                ImGui::PushFont(gfx->fontScalable);
                float scale = camera->zoom / 256.0f * e.labelSize.get();
                ImGui::SetWindowFontScale(scale);
                auto labelBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);

                gfn::Vec2 midpoint((e.startVertexPosition.get() + e.endVertexPosition.get()) / 2.0);
                gfn::Vec2 vector(e.endVertexPosition.get() - e.startVertexPosition.get());
                gfn::Vec2 location(midpoint + (vector.rotate(M_PI_2).normalize() * camera->rMap(std::sqrt(labelBB.x * labelBB.x + labelBB.y * labelBB.y)) / 2.0));

                drawList->AddText(ImVec2(camera->map(location).x - labelBB.x / 2.0f, camera->map(location).y - labelBB.y / 2.0f * 1.1f),
                                  e.labelColor.value, label.c_str());
                ImGui::PopFont();
                ImGui::SetWindowFontScale(1.0f);
            }
        }

        void drawVertices(gfn::Graphics* gfx) {
            ZoneScoped
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = itf->graph.getRead()->props;
            for (auto& vi : userprops.getVertexPropsList()) {
                auto v = vi.second;
                if (!v.enabled.get())
                    continue;

                drawList->AddCircleFilled(camera->map(v.position.get()), camera->map(v.radius.get()),
                                          v.fillColor.get(), 0);
                drawList->AddCircle(camera->map(v.position.get()), camera->map(v.radius.get()),
                                    v.outlineColor.get(), 0, camera->map(v.outlineThickness.get()));

                std::string label = vi.second.label.get();
                ImGui::PushFont(gfx->fontScalable);
                float scale;
                if (v.labelSize.get() < 0) {
                    auto fontBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);
                    scale = camera->zoom / 256.0f / std::sqrt(fontBB.x * fontBB.x + fontBB.y * fontBB.y) * 256.0f * 2.0 *
                            (v.radius.get() - v.outlineThickness.get() / 2.0f) * 0.9;
                } else {
                    scale = camera->zoom / 256.0f * v.labelSize.get();
                }
                ImGui::SetWindowFontScale(scale);
                auto labelBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);

                drawList->AddText(ImVec2(camera->map(v.position.value).x - labelBB.x / 2.0f, camera->map(v.position.value).y - labelBB.y / 2.0f * 1.1f),
                                  v.labelColor.value, label.c_str());
                ImGui::PopFont();
                ImGui::SetWindowFontScale(1.0f);
            }
        }
    };
}