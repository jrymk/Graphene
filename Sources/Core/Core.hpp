#pragma once

#include <Graph/Graph.h>
#include <Structure/Structure.h>

#include <Configs/Configs.hpp>
#include <Interface/Interface.hpp>
#include <Core/Drawlgo/Drawlgo.hpp>
#include <Core/CmdBuffer.hpp>
#include <atomic>
#include <iomanip>
#include <Core/Parser.hpp>
#include <minitrace.h>

namespace gfn::core {
    class Core {
    public:
        gfn::Interface itf;
        gfn::Structure structure;
        gfn::core::drawlgo::Drawlgo drawlgo;
        gfn::parser::Parser parser;

        explicit Core() :
                itf(),
                structure(&itf),
                parser(&itf) {
        }

        gfn::Interface* getInterface() {
            return &itf;
        }

        void coreCycle() {
            MTR_META_PROCESS_NAME("graphene-core");
            MTR_META_THREAD_NAME("core thread");
            MTR_SCOPE("core", "core cycle");

            parser.parseAll();

            if (itf.graph.getWrite().pendingUpdate) {
                // update component list with usergraph and rebuild block cut tree
                structure.componentify();
                itf.graph.getWrite().pendingUpdate = false;
                /// TODO: partial update
            }

            drawlgo.update(&itf, &structure);

            if (itf.props.wantWrite()) {
                itf.props.writeDone();
            }

            itf.cfg.writeDone();
        }
    };
} // namespace gfn::core