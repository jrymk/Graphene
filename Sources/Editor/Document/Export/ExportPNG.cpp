#include "../Document.h"
#include <Editor/Theme/Theme.h>
#include <simple_svg_1.0.0.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>
#include <svgdocument.h>

namespace gfn {
    void Document::exportPNG() {
        static std::string fontSrc;
        ImGui::InputText("Font", fontSrc.data(), 1024);
        static int width = 4096, height = 4096;
        ImGui::InputInt("Width", &width, 1, 10);
        width = std::max(1, width);
        ImGui::InputInt("Height", &height, 1, 10);
        height = std::max(1, height);

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 64.0f);

        if (gfn::button("\ue161 Save PNG", HUE_BLUE_CONTRAST, HUE_CONTRAST, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
            svg::Document svg("temp.svg",
                              svg::Layout(svg::Dimensions(graphview.camera.xMax - graphview.camera.xMin, graphview.camera.yMax - graphview.camera.yMin),
                                          svg::Layout::BottomLeft));
            gfn::Vec2 origin(graphview.camera.xMin, graphview.camera.yMin);
            // draw edges
            for (auto& e : itf->graph.getRead()->props.getEdgePropsList()) {
                if (!e.second.enabled.value)
                    continue;
                gfn::Vec2 u = e.second.startVertexPosition.value + (e.second.endVertexPosition.value - e.second.startVertexPosition.value).normalize() *
                                                                   (itf->graph.getRead()->props.getVertexProps(
                                                                           e.second.startVertexUuid.value)->radius.value +
                                                                    itf->graph.getRead()->props.getVertexProps(
                                                                            e.second.startVertexUuid.value)->outlineThickness.value / 2.0);
                gfn::Vec2 ep = e.second.position.value;
                double arrowCompensation = itf->graph.getRead()->props.getVertexProps(e.second.endVertexUuid.value)->radius.value +
                                           itf->graph.getRead()->props.getVertexProps(e.second.endVertexUuid.value)->outlineThickness.value / 2.0;
                gfn::Vec2 v =
                        u + (e.second.endVertexPosition.value - u).normalize() * ((e.second.endVertexPosition.value - u).length() - arrowCompensation);

                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(e.second.edgeColor.value);
                gfn::Vec2 edgeEndpointA = u + (v - u).normalize() * ((e.second.arrowStyle.value & 0b10) * 0.1 * (e.second.thickness.value / 0.06));
                gfn::Vec2 edgeEndpointB = u + (v - u).normalize() * ((v - u).length() -
                                                                     (e.second.arrowStyle.value & 0b1) * 0.1 * (e.second.thickness.value / 0.06));

                svg << svg::Line(svg::Point(edgeEndpointA.x - origin.x, edgeEndpointA.y - origin.y),
                                 svg::Point(edgeEndpointB.x - origin.x, edgeEndpointB.y - origin.y),
                                 svg::Stroke(e.second.thickness.value,
                                             svg::Color(int(rgba.x * 255.0 + 0.5), int(rgba.y * 255.0 + 0.5), int(rgba.z * 255.0 + 0.5))));

                static gfn::Vec2 arrowHead[] = {
                        {0.0,  0.0},
                        {-0.3, -0.13},
                        {-0.2, 0.0},
                        {-0.3, 0.13}
                };

                if (e.second.arrowStyle.value & 0b1) {
                    std::vector<gfn::Vec2> transformedArrowHead;
                    double theta = acos((v.x - u.x) / (v - u).length()) * (v.y - u.y >= 0 ? -1.0 : 1.0);
                    for (auto& p : arrowHead)
                        transformedArrowHead.emplace_back((p * (e.second.thickness.value / 0.06)).rotate(theta) + v - origin);
                    svg << (svg::Polygon(svg::Fill(svg::Color(int(rgba.x * 255.0 + 0.5), int(rgba.y * 255.0 + 0.5), int(rgba.z * 255.0 + 0.5))),
                                         svg::Stroke())
                            << svg::Point(transformedArrowHead[0].x, transformedArrowHead[0].y)
                            << svg::Point(transformedArrowHead[1].x, transformedArrowHead[1].y)
                            << svg::Point(transformedArrowHead[2].x, transformedArrowHead[2].y)
                            << svg::Point(transformedArrowHead[3].x, transformedArrowHead[3].y)
                            << svg::Point(transformedArrowHead[0].x, transformedArrowHead[0].y)
                    );
                }
                if (e.second.arrowStyle.value & 0b10) {
                    std::vector<gfn::Vec2> transformedArrowHead;
                    double theta = acos((u.x - v.x) / (u - v).length()) * (u.y - v.y >= 0 ? -1.0 : 1.0);
                    for (auto& p : arrowHead)
                        transformedArrowHead.emplace_back((p * (e.second.thickness.value / 0.06)).rotate(theta) + u - origin);
                    svg << (svg::Polygon(svg::Fill(svg::Color(int(rgba.x * 255.0 + 0.5), int(rgba.y * 255.0 + 0.5), int(rgba.z * 255.0 + 0.5))),
                                         svg::Stroke())
                            << svg::Point(transformedArrowHead[0].x, transformedArrowHead[0].y)
                            << svg::Point(transformedArrowHead[1].x, transformedArrowHead[1].y)
                            << svg::Point(transformedArrowHead[2].x, transformedArrowHead[2].y)
                            << svg::Point(transformedArrowHead[3].x, transformedArrowHead[3].y)
                            << svg::Point(transformedArrowHead[0].x, transformedArrowHead[0].y)
                    );
                }

                std::string label = e.second.label.value;
                ImGui::PushFont(graphview.gfx->fontSvg);
                float scale = e.second.labelSize.value;
                ImGui::SetWindowFontScale(scale);
                auto labelBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);
                ImVec4 labelRgba = ImGui::ColorConvertU32ToFloat4(e.second.labelColor.value);

                gfn::Vec2 midpoint((e.second.startVertexPosition.value + e.second.endVertexPosition.value) / 2.0);
                gfn::Vec2 vector(e.second.endVertexPosition.value - e.second.startVertexPosition.value);
                gfn::Vec2 location(midpoint + (vector.rotate(M_PI_2).normalize() *
                                               (std::sqrt(labelBB.x * labelBB.x + labelBB.y * labelBB.y) / 512.0 + e.second.thickness.value / 2.0)));

                svg << svg::Text(
                        svg::Point(location.x - origin.x - labelBB.x / 450.0f, location.y - origin.y - labelBB.y / 700.0f),
                        e.second.label.value,
                        svg::Color(int(labelRgba.x * 255.0 + 0.5), int(labelRgba.y * 255.0 + 0.5), int(labelRgba.z * 255.0 + 0.5)),
                        svg::Font(scale, "Arial"));

                ImGui::PopFont();
                ImGui::SetWindowFontScale(1.0f);
            }

            // draw vertices
            for (auto& v : itf->graph.getRead()->props.getVertexPropsList()) {
                if (!v.second.enabled.value)
                    continue;

                ImVec4 fillRgba = ImGui::ColorConvertU32ToFloat4(v.second.fillColor.value);
                ImVec4 outlineRgba = ImGui::ColorConvertU32ToFloat4(v.second.outlineColor.value);
                ImVec4 labelRgba = ImGui::ColorConvertU32ToFloat4(v.second.labelColor.value);

                svg << svg::Circle(svg::Point(v.second.position.value.x - origin.x, v.second.position.value.y - origin.y),
                                   v.second.radius.value * 2.0,
                                   svg::Color(int(fillRgba.x * 255.0 + 0.5), int(fillRgba.y * 255.0 + 0.5), int(fillRgba.z * 255.0 + 0.5)),
                                   svg::Stroke(v.second.outlineThickness.value,
                                               svg::Color(int(outlineRgba.x * 255.0 + 0.5), int(outlineRgba.y * 255.0 + 0.5),
                                                          int(outlineRgba.z * 255.0 + 0.5))));

                std::string label = v.second.label.value;
                ImGui::PushFont(graphview.gfx->fontSvg);
                float scale;
                if (v.second.labelSize.value < 0) {
                    auto fontBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);
                    scale = 1.0 / std::sqrt(fontBB.x * fontBB.x + fontBB.y * fontBB.y) * 256.0f * 2.0 *
                            (v.second.radius.value - v.second.outlineThickness.value / 2.0f) * 0.9;
                } else {
                    scale = v.second.labelSize.value;
                }
                ImGui::SetWindowFontScale(scale);
                auto labelBB = ImGui::CalcTextSize(label.c_str(), nullptr, false, -1);
                ImGui::SetWindowFontScale(1.0f);
                ImGui::PopFont();

                svg << svg::Text(
                        svg::Point(v.second.position.value.x - origin.x - labelBB.x / 450.0f, v.second.position.value.y - origin.y - labelBB.y / 700.0f),
                        v.second.label.value,
                        svg::Color(int(labelRgba.x * 255.0 + 0.5), int(labelRgba.y * 255.0 + 0.5), int(labelRgba.z * 255.0 + 0.5)),
                        svg::Font(scale, "Arial"));
            }
            svg.save();

            lunasvg::SVGDocument document;
            document.loadFromFile("temp.svg");
            document.loadFontFromFile(fontSrc);
            auto bitmap = document.renderToBitmap(width, height, 0x00000000);
            std::string basename = "pngexport.png";
            stbi_write_png(basename.c_str(), int(bitmap.width()), int(bitmap.height()), 4, bitmap.data(), 0);
        }
        if (gfn::button("\ue876 Done", HUE_GREEN, HUE_DEFAULT, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
            showPNGMenu = false;
            ImGui::CloseCurrentPopup();
        }
    }
}