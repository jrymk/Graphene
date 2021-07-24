#pragma once

#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Structure.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Parser/Parser.hpp>
#include <Interface/Interface.hpp>
#include <atomic>

namespace gfn::core {
class Core {
  public:
	std::atomic_bool pendingOutput = false;

	gfn::core::properties::Properties properties;
	gfn::core::usergraph::UserGraph usergraph;
	gfn::core::structure::Structure structure;
	gfn::interface::Content content;
	//gfn::core::parser::Parser parser;

	Core() {
		//parser.usergraph = &usergraph;
		usergraph.bindProperties(&properties);
		structure.componentList.bindProperties(&properties);
	}

	void coreCycle(gfn::interface::Interface* interface) {
        //parser.execute("graph new vertex");
		if (usergraph.pendingUpdate) {
			// update component list with usergraph and rebuild block cut tree (currently all)
			structure.componentList.componentify(&usergraph);
            usergraph.pendingUpdate = false;
		}

		/// TODO: graph update stuff
        std::cerr << usergraph.getAdjList().size() << "\n";

        /*if (pendingOutput) {
			content.properties = properties;
			interface->buffer.put(content);
			pendingOutput = false;
		}*/
	}
};
} // namespace gfn::core