#pragma once

#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Structure.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Parser/Parser.hpp>

namespace gfn::core {
class Core {
  public:
	gfn::core::properties::Properties properties;
	gfn::core::usergraph::UserGraph usergraph;
	gfn::core::structure::Structure structure;

	gfn::core::parser::Parser parser;

	Core() {
		parser.usergraph = &usergraph;
		usergraph.bindProperties(&properties);
		structure.componentList.bindProperties(&properties);
	}
};
} // namespace gfn::core