#pragma once

#include <sstream>
#include <string>
#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Objects/Uuid.hpp>

namespace gfn::core::parser {
typedef std::string Command;

class Parser {
  public:
	Parser() = default;

	gfn::core::usergraph::UserGraph* usergraph;

	gfn::core::Uuid execute(Command cmd) {
		// std::cout << "Command: " << cmd << "\n";
		std::stringstream ss;
		std::string arg;
		ss << cmd;
		ss >> arg;
		if (arg == "graph") { // graph
			ss >> arg;
			if (arg == "new") { // graph new
				ss >> arg;
				if (arg == "vertex") { // graph new vertex
					if (ss >> arg) {
						if (gfn::core::uuid::isUuid(arg)) { // graph new vertex 00000000-0000-0000-0000-000000000000
							if (ss >> arg) {
								// graph new vertex 00000000-0000-0000-0000-000000000000 ***
							} else
								usergraph->addVertex(arg);
						}
					} else // graph new vertex
						return usergraph->addVertex().second;
				} else if (arg == "edge") {
					ss >> arg;
					if (gfn::core::uuid::isUuid(arg)) {
						::std::string u = arg;
						ss >> arg;
						if (gfn::core::uuid::isUuid(arg)) {
							::std::string v = arg;
							if (ss >> arg) {
								if (gfn::core::uuid::isUuid(arg)) // graph new edge 000-000 000-000 000-000
									usergraph->addEdge(u, v, arg);
								else {
									// graph new edge 000-000 000-000 ***
								}
							} else
								return usergraph->addEdge(u, v).second; // graph new edge 000-000 000-000
						} else {
							// graph new edge 000-000 *** ***
						}
					} else {
						// graph new edge *** *** ***
					}
				} else if (arg == "remove") { // graph remove
				}
			}
		}
		return gfn::core::uuid::createNil();
	}
};
} // namespace gfn::core::parser