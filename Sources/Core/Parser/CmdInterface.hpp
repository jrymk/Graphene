#pragma once

#include <sstream>
#include <string>
#include <UserGraph/UserGraph.hpp>
#include <Objects/Uuid.hpp>
#include <Objects/Command.hpp>

namespace gfn::core::cmd {

class CmdInterface {
  public:
	CmdInterface() = default;

	gfn::usergraph::UserGraph* usergraph;

	gfn::Uuid execute(const gfn::Command& cmd) {
		std::cout << "Command: " << cmd << "\n";

		gfn::command::Parser parser(cmd);
		if (parser.readWord() == "graph") { // graph
            usergraph->execute(parser.getRemaining());
		}

		return gfn::uuid::createNil();
	}
};
} // namespace gfn::core::parser