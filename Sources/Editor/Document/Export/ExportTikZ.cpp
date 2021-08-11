#include "../Document.h"
#include <regex>

namespace gfn {
    std::string Document::exportTikZ() {
        std::string picture;

        double scale = 0.35;

        picture.append("\\begin{tikzpicture}[scale=" + std::to_string(scale) + "]\n");

        // draw edges
        for (auto& e : itf->graph.getRead()->props.getEdgePropsList()) {
            if (!e.second.enabled.value)
                continue;
            gfn::Vec2 u = e.second.startVertexPosition.value + (e.second.endVertexPosition.value - e.second.startVertexPosition.value).normalize() *
                                                               (itf->graph.getRead()->props.getVertexProps(e.second.startVertexUuid.value)->radius.value +
                                                                itf->graph.getRead()->props.getVertexProps(
                                                                        e.second.startVertexUuid.value)->outlineThickness.value / 2.0);
            gfn::Vec2 ep = e.second.position.value;
            double arrowCompensation = itf->graph.getRead()->props.getVertexProps(e.second.endVertexUuid.value)->radius.value +
                                       itf->graph.getRead()->props.getVertexProps(e.second.endVertexUuid.value)->outlineThickness.value / 2.0;
            gfn::Vec2 v = u + (e.second.endVertexPosition.value - u).normalize() * ((e.second.endVertexPosition.value - u).length() - arrowCompensation);

            picture.append("\\draw[color=" + _tikzColor(e.second.edgeColor.value) + ", line width=" + std::to_string(e.second.thickness.value * scale) + "cm]");
            if (e.second.arrowStyle.value == 0b01) picture.append("[-stealth]");
            else if (e.second.arrowStyle.value == 0b10) picture.append("[stealth-]");
            else if (e.second.arrowStyle.value == 0b11) picture.append("[stealth-stealth]");
            picture.append(" (" + std::to_string(u.x) + ", " + std::to_string(u.y) + ")");
            picture.append(" -- ");
            picture.append("(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")");
            picture.append(";\n");

            gfn::Vec2 midpoint((e.second.startVertexPosition.value + e.second.endVertexPosition.value) / 2.0);
            gfn::Vec2 vector(e.second.endVertexPosition.value - e.second.startVertexPosition.value);
            gfn::Vec2 location(
                    midpoint + (vector.rotate(M_PI_2).normalize() * 0.8/*PARAM*/ / 2.0));
            //\node[draw,align=left] at (3,0) {some text\\ spanning three lines\\ with manual line breaks};
            picture.append("\\node[align=center, font=\\tiny, text=" + _tikzColor(e.second.labelColor.value) + "] at ");
            //            picture.append(std::to_string(v.second.radius.value * 2.0));
            //            picture.append("cm] at ");
            picture.append("(" + std::to_string(location.x) + ", " + std::to_string(location.y) + ")");
            picture.append(" {");
            picture.append(std::regex_replace(e.second.label.value, std::regex("\n"), "\\\\"));
            picture.append("};\n");
        }

        // draw vertices
        for (auto& v : itf->graph.getRead()->props.getVertexPropsList()) {
            if (!v.second.enabled.value)
                continue;
            picture.append("\\draw[color=black, fill=" + _tikzColor(v.second.fillColor.value) + ", line width=" +
                           std::to_string(v.second.outlineThickness.value * scale) + "cm] ");

            picture.append("(" + std::to_string(v.second.position.value.x) + ", " + std::to_string(v.second.position.value.y) + ")");
            picture.append(" circle ");
            picture.append("(" + std::to_string(v.second.radius.value) + ")");
            picture.append(";\n");

            //\node[draw,align=left] at (3,0) {some text\\ spanning three lines\\ with manual line breaks};
            picture.append("\\node[align=center, font=\\scriptsize, text=" + _tikzColor(v.second.labelColor.value) + "] at ");
//            picture.append(std::to_string(v.second.radius.value * 2.0));
//            picture.append("cm] at ");
            picture.append("(" + std::to_string(v.second.position.value.x) + ", " + std::to_string(v.second.position.value.y) + ")");
            picture.append(" {");
            picture.append(std::regex_replace(v.second.label.value, std::regex("\n"), "\\\\"));
            picture.append("};\n");
        }

        picture.append("\\end{tikzpicture}");

        ImGui::SetClipboardText(picture.c_str());
        std::cout << "Copied to clipboard\n";
        //std::cout << picture << "\n";
        return picture;
    }

    std::string Document::_tikzColor(uint32_t color) {
        std::string str;
        str.append("{rgb,255:red," + std::to_string(int(ImGui::ColorConvertU32ToFloat4(color).x * 255.0f + 0.5f)) + ";");
        str.append("green," + std::to_string(int(ImGui::ColorConvertU32ToFloat4(color).y * 255.0f + 0.5f)) + ";");
        str.append("blue," + std::to_string(int(ImGui::ColorConvertU32ToFloat4(color).z * 255.0f + 0.5f)) + "}");
        return str;
    }
}