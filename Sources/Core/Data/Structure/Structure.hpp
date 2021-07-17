#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <Core/Data/Properties/Uuid.hpp>
#include <Core/Data/Properties/Properties.hpp>

#include <Core/Data/Structure/ConnectedComponent.hpp>

namespace gfn::core::data {
// files in this folder maintains the structure of the graph, including updating the graph
// from input, identify components, building block cut tree and more.
class Structure {
  public:
	std::unordered_map<gfn::core::Uuid, std::unordered_multimap<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>>
		adjList;
};

}; // namespace gfn::core::data