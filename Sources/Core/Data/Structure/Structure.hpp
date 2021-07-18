#pragma once

#include <Core/Data/Properties/Properties.hpp>
#include <Core/Data/Structure/AdjacencyList.hpp>
#include <Core/Data/Structure/ConnectedComponent.hpp>
#include <Core/Tasks/Tasks.hpp>

namespace gfn::core::data {
class Structure {
  public:
	gfn::core::data::AdjacencyList adjacencyList;
	gfn::core::data::ComponentList componentList;
	gfn::core::data::Properties* properties;
	gfn::core::TaskManager* coreTaskManager;

	Structure() {
		adjacencyList.bindTaskManager(coreTaskManager);
		componentList.bindSource(&adjacencyList);
		componentList.bindProperties(properties);
		componentList.bindTaskManager(coreTaskManager);
	}
};
} // namespace gfn::core::data