#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <Core/Data/Properties/Uuid.hpp>
#include <Core/Data/Properties/Properties.hpp>

namespace gfn::core::data {
class Structure {
  public:
	std::unordered_map<gfn::core::Uuid, std::unordered_multimap<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>>
		adjList;
	// keep v removed if no edges
};

}; // namespace gfn::core::data