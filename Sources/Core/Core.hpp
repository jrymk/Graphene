#pragma once

#include <Core/Logging/Logging.hpp>
#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Structure.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Parser/Parser.hpp>
#include <Interface/Interface.hpp>
#include <atomic>

namespace gfn::core {
class Core {
  public:
	gfn::core::properties::Properties properties;
	gfn::core::usergraph::UserGraph usergraph;
	gfn::core::structure::Structure structure;
	gfn::core::parser::Parser parser;

	Core() {
		usergraph.bindProperties(&properties);
		structure.componentList.bindProperties(&properties);
		parser.usergraph = &usergraph;
	}

	void coreCycle(gfn::interface::Interface* interface) {
		static bool first = true;
		if (first) {
			for (int i = 0; i < 1000; i++) {
				parser.execute("graph new vertex");
			}
			first = false;
		}

		if (usergraph.pendingUpdate) {
			// update component list with usergraph and rebuild block cut tree (currently all)
			structure.componentList.componentify(&usergraph);
			usergraph.pendingUpdate = false;
		}

		// std::cerr << usergraph.getAdjList().size() << "\n";
		/// TODO: graph update stuff

		if (interface->properties.pendingWrite()) {
			// assignment operator, writes the core content to the write buffer
			interface->properties.getWriteBuffer() = properties;
			interface->properties.writeDone();
		}

		if (interface->logBuffer.pendingWrite()) {
			// assignment operator, writes the core content to the write buffer
			interface->logBuffer.getWriteBuffer() = gfn::core::logging::logBuffer;
			interface->logBuffer.writeDone();
		}
	}
};
} // namespace gfn::core