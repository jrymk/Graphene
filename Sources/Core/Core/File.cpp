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
}

