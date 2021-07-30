#pragma once

#include <sstream>
#include <string>
#include <UserGraph/UserGraph.hpp>
#include <Objects/Uuid.hpp>
#include <Objects/Command.hpp>
#include <utility>

namespace gfn::core::cmd {

    class CmdInterface {
    public:
        CmdInterface() = default;

        gfn::usergraph::UserGraph* usergraph;

        gfn::Uuid execute(gfn::Command command, gfn::Command& output) {
            usergraph->tryParse(std::move(command), output);

            return gfn::uuid::createNil();
        }
    };
} // namespace gfn::core::parser