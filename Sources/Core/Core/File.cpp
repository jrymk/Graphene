#include "Core.h"

#include <iostream>
#include <fstream>

namespace gfn {
    void Core::openFile(Args command, gfn::Args& output) {
        std::string filePath = command.getParamValue("-f");
        if (filePath.empty())
            filePath = file;
        std::ifstream gfnFile(filePath.c_str(), std::ios::in | std::ios::binary);
        if (gfnFile) {
            // obtain file size
            gfnFile.seekg(0, std::ifstream::end);
            std::ifstream::pos_type size = gfnFile.tellg();
            gfnFile.seekg(0, std::ifstream::beg);
            // read buffer
            binn* document;
            char* buffer = new char[size];
            gfnFile.read(buffer, size);
            document = binn_open(buffer);
            output.newParam("-output", filePath + "  File size: " + std::to_string(size) +
                                       " bytes (Read " + std::to_string(binn_size(document)) + " bytes)");
            if (size != binn_size(document)) {
                output.newParam("-successful", "false");
                output.newParam("-error", "READ_FILE_FAIL");
            }
            gfnFile.close();
            /// DESERIALIZE
            itf.graph.getWrite().props.deserialize(document);
            itf.graph.getWrite().deserialize(document);
            itf.graph.getWrite().cfg.deserialize(document);
            /// DESERIALIZE
            // clean up
            output.newParam("-message", "Read " + std::to_string(binn_size(document)) + " bytes from " + filePath);
            binn_free(document);
            delete[] buffer;
            output.newParam("-successful", "true");
        } else {
            output.newParam("-successful", "false");
            output.newParam("-error", "READ_FILE_FAIL");
        }
    }

    void Core::saveFile(Args command, gfn::Args& output) {
        std::string filePath = command.getParamValue("-f");
        if (filePath.empty())
            filePath = file;
        std::ofstream gfnFile(filePath.c_str(), std::ios::out | std::ios::binary);
        //if (gfnFile) {
        // allocate memory
        binn* document = binn_object();
        /// SERIALIZE
        itf.graph.getWrite().props.serialize(document);
        itf.graph.getWrite().serialize(document);
        itf.graph.getWrite().cfg.serialize(document);
        /// SERIALIZE
        output.newParam("-output", filePath + "  Output size: " +
                                   std::to_string(binn_size(document)) + " bytes");
        // write file and clean up
        gfnFile.write((char*) binn_ptr(document), binn_size(document));
        gfnFile.close();
        output.newParam("-message", "Writen " + std::to_string(binn_size(document)) + " bytes to " + filePath);
        binn_free(document);
        output.newParam("-successful", "true");
        //} else {
        //    output.newParam("-successful", "false");
        //    output.newParam("-error", "READ_FILE_FAIL");
        //}
    }

    /*void Core::paste(const std::string& pasteContent) {
        gfn::Uuid pasteId = gfn::createUuid();
        nlohmann::json j = nlohmann::json::parse(pasteContent);
        for (auto& sj : j.items()) {
            std::cerr << sj.key() << "\n";
            if (sj.key() == "vertex props") {

                for (auto& vProp : sj.value().items()) {
                    gfn::Uuid vUuid = gfn::createUuid();
                    gfn::Args output;
                    itf.graph.getWrite().addVertex(gfn::Args("mkvertex -uuid=" + vUuid + " -name=" + pasteId + "-v" + vProp.key()), output);
                    auto vFind = itf.graph.getWrite().props.getVertexPropsList().find(vUuid);
                    if (vFind == itf.graph.getWrite().props.getVertexPropsList().end()) {
                        std::cerr << "Paste failed: VERTEX_CREATION_FAILED\n";
                        return;
                    }
                    auto va = vProp.value().find(vProp.key());
                    vFind->second.deserializeJson(va.value());

                    std::cout << va.value() << "\n";
                    return;
                }
            }
        }
        std::cerr << "Paste failed: PARSE_ERROR\n";
        return;*/


    /*for (auto&[u, value] : j["structure"].items()) {
        for (auto& ve : value.items()) {
            gfn::Uuid eUuid = gfn::createUuid();
            gfn::Args output;
            std::string e = ve.key();
            std::string v = ve.value();
            std::string uName = pasteId + "-v" + u;
            std::string vName = pasteId + "-v" + v;
            itf.graph.getWrite().addEdge(gfn::Args("mkedge -uuid=" + eUuid + "-e" + e + " -uname=" + uName + " -vname=" + vName), output);
            auto eFind = itf.graph.getWrite().props.getEdgePropsList().find(eUuid);
            if (eFind == itf.graph.getWrite().props.getEdgePropsList().end()) {
                std::cerr << "Paste failed: EDGE_CREATION_FAILED\n";
                return;
            }
            eFind->second.deserializeJson(j["edge props"][e]);
            eFind->second.startVertexUuid.get() = itf.graph.getWrite().props.convertAccessName(uName);
            eFind->second.endVertexUuid.get() = itf.graph.getWrite().props.convertAccessName(vName);
        }
    }*/
    //}
}

