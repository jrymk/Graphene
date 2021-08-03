#pragma once

#include <fstream>
#include <iostream>
#include <binn.h>

namespace gfn::parser::file {
    void open(Command command, gfn::Command& output, gfn::props::Properties* properties,
              gfn::usergraph::UserGraph* usergraph, gfn::configs::Configs* configs) {
        std::string filePath = command.getParamValue("-f");
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
            output.newParam("-output", filePath + "  File size: " + std::to_string(size) + " bytes (Read " +
                                       std::to_string(binn_size(document)) + " bytes)");
            if (size != binn_size(document)) {
                output.newParam("-successful", "false");
                output.newParam("-error", "READ_FILE_FAIL");
            }
            gfnFile.close();
            // deserialize
            properties->deserialize(document);
            usergraph->deserialize(document);
            configs->deserialize(document);
            // clean up
            binn_free(document);
            delete[] buffer;
            output.newParam("-successful", "true");
        } else {
            output.newParam("-successful", "false");
            output.newParam("-error", "READ_FILE_FAIL");
        }
    }

    void save(Command command, gfn::Command& output, gfn::props::Properties* properties,
              gfn::usergraph::UserGraph* usergraph, gfn::configs::Configs* configs) {
        std::string filePath = command.getParamValue("-f");
        std::ofstream gfnFile(filePath.c_str(), std::ios::out | std::ios::binary);
        if (gfnFile) {
            // allocate memory
            binn* document = binn_object();
            // serialize
            properties->serialize(document);
            usergraph->serialize(document);
            configs->serialize(document);
            output.newParam("-output", filePath + "  Output size: " + std::to_string(binn_size(document)) +
                                       " bytes");
            // write file and clean up
            gfnFile.write((char*) binn_ptr(document), binn_size(document));
            gfnFile.close();
            binn_free(document);
            output.newParam("-successful", "true");
        } else {
            output.newParam("-successful", "false");
            output.newParam("-error", "READ_FILE_FAIL");
        }
    }
}