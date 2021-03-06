#pragma once

#include <Core/Objects/Serializable.h>
#include <binn.h>

namespace gfn::configs {
    class Configs {
    public:
        gfn::serializable::Double c1;
        gfn::serializable::Double c2;
        gfn::serializable::Double c3;
        gfn::serializable::Double c4;
        gfn::serializable::Double c5;
        gfn::serializable::Double c6;
        gfn::serializable::Double c7;
        gfn::serializable::Double c8;
        gfn::serializable::Double c9;
        gfn::serializable::Bool doGraphUpdate;
        bool energySavingMode = true;
        bool qualityMode = false;

        Configs() :
                c1("c1", 0.1), // edge attract coeff
                c2("c2", 1.0), // preferred edge length
                c3("c3", 1.0), // repel force coeff
                c4("c4", 0.8), // c1 - c3 coeff
                c5("c5", 1.0), // edge attract coeff
                c6("c6", 0.1), // preferred edge to vertex distance
                c7("c7", 20.0), // multi edge (edge to edge) repel force coeff
                c8("c8", 0.01), // edge to vertex repel force coeff
                c9("c9", 0.05), // c5 - c8 coeff
                doGraphUpdate("doGraphUpdate", false) {
        }

        bool parse(gfn::Args command, gfn::Args& output) {
            if (command.getParamValue("command") == "configs") {
                if (!command.getParamValue("-c1").empty())
                    c1.setValueStr(command.getParamValue("-c1"), output);
                if (!command.getParamValue("-c2").empty())
                    c2.setValueStr(command.getParamValue("-c2"), output);
                if (!command.getParamValue("-c3").empty())
                    c3.setValueStr(command.getParamValue("-c3"), output);
                if (!command.getParamValue("-c4").empty())
                    c4.setValueStr(command.getParamValue("-c4"), output);
                if (!command.getParamValue("-c5").empty())
                    c5.setValueStr(command.getParamValue("-c5"), output);
                if (!command.getParamValue("-c6").empty())
                    c6.setValueStr(command.getParamValue("-c6"), output);
                if (!command.getParamValue("-c7").empty())
                    c7.setValueStr(command.getParamValue("-c7"), output);
                if (!command.getParamValue("-c8").empty())
                    c8.setValueStr(command.getParamValue("-c8"), output);
                if (!command.getParamValue("-c9").empty())
                    c9.setValueStr(command.getParamValue("-c9"), output);
                if (command.getFlag("-pause"))
                    doGraphUpdate.setValueStr("false", output);
                if (command.getFlag("-resume")) {
                    doGraphUpdate.setValueStr("true", output);
                    energySavingMode = false;
                }
                if (command.getFlag("-performance"))
                    qualityMode = false;
                if (command.getFlag("-quality"))
                    qualityMode = true;

                return true;
            }
            return false;
        }

        void serialize(binn* document) {
            binn* cfg = binn_object();
            binn_object_set_double(cfg, c1.key.c_str(), c1.value);
            binn_object_set_double(cfg, c2.key.c_str(), c2.value);
            binn_object_set_double(cfg, c3.key.c_str(), c3.value);
            binn_object_set_double(cfg, c4.key.c_str(), c4.value);
            binn_object_set_double(cfg, c5.key.c_str(), c5.value);
            binn_object_set_double(cfg, c6.key.c_str(), c6.value);
            binn_object_set_double(cfg, c7.key.c_str(), c7.value);
            binn_object_set_double(cfg, c8.key.c_str(), c8.value);
            binn_object_set_double(cfg, c9.key.c_str(), c9.value);
            binn_object_set_object(document, "configs", cfg);
            binn_free(cfg);
        }

        void deserialize(void* document) {
            void* cfg = binn_object_list(document, "configs");
            c1.value = binn_object_double(cfg, c1.key.c_str());
            c2.value = binn_object_double(cfg, c2.key.c_str());
            c3.value = binn_object_double(cfg, c3.key.c_str());
            c4.value = binn_object_double(cfg, c4.key.c_str());
            c5.value = binn_object_double(cfg, c5.key.c_str());
            c6.value = binn_object_double(cfg, c6.key.c_str());
            c7.value = binn_object_double(cfg, c7.key.c_str());
            c8.value = binn_object_double(cfg, c8.key.c_str());
            c9.value = binn_object_double(cfg, c9.key.c_str());
        }
    };
}