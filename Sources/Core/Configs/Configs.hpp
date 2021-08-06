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

        Configs() :
                c1("c1", 2.0),
                c2("c2", 1.0),
                c3("c3", 1.0),
                c4("c4", 0.12),
                c5("c5", 0.1),
                c6("c6", 0.0001) {
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
                return false;
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
        }
    };
}