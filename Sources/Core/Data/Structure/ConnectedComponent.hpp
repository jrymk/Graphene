#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <Core/Data/Uuid.hpp>
#include <Core/Data/Properties/Properties.hpp>
#include <Core/Data/Uuid.hpp>
#include <Core/Data/Structure/AdjacencyList.hpp>
#include <System/Logging/Logging.hpp>

namespace gfn::core::data {

class ConnectedComponent {
  public:
	gfn::core::Uuid uuid;
	gfn::core::Uuid root;
	std::unordered_set<gfn::core::Uuid> vertices;

	ConnectedComponent(gfn::core::Uuid _uuid) : uuid(_uuid) {}
};

class ComponentList {
  private:
	gfn::core::TaskManager* taskManager;
	gfn::core::data::Properties* properties;
	gfn::core::data::AdjacencyList* source;

	std::unordered_map<gfn::core::Uuid, gfn::core::data::ConnectedComponent> components;

	void updateDfs(gfn::core::data::ConnectedComponent& component, gfn::core::Uuid u,
				   std::unordered_set<gfn::core::Uuid>& remaining) {
		component.vertices.insert(u);
		properties->getVertexProp(u).component = component.uuid;

		auto uIt = source->getRawList().find(u);
		if (uIt == source->getRawList().end()) {
			logInsert("core::data::ComponentList: update component list failed to access vID ");
			logInsert(u);
			logWarning(", the following vertex will be ignored");
		}

		for (auto& v : uIt->second) {
			remaining.erase(v.first);
			updateDfs(component, v.first, remaining);
		}
	}

  public:
	void bindTaskManager(gfn::core::TaskManager* taskManager) { this->taskManager = taskManager; }

	void bindProperties(gfn::core::data::Properties* properties) { this->properties = properties; }

	void bindSource(gfn::core::data::AdjacencyList* src) { source = src; }

	void update() {
		std::unordered_set<gfn::core::Uuid> remaining;
		for (auto& v : source->getRawList())
			remaining.insert(v.first);

		for (auto cIt = components.begin(); cIt != components.end(); cIt++) {
			if (remaining.find(cIt->second.root) == remaining.end()) { // component has merged
				properties->removeComponentProp(cIt->second.uuid);
				cIt = components.erase(cIt);
			}
			updateDfs(cIt->second, cIt->second.root, remaining);
		}

		// generate new components for those who don't have one
		for (auto vIt = remaining.begin(); vIt != remaining.end(); vIt++) {
			gfn::core::Uuid uuid = gfn::system::uuid::createUuid();
			components.insert(std::make_pair(uuid, gfn::core::data::ConnectedComponent(uuid)));

			updateDfs(components.find(uuid)->second, *vIt, remaining);
		}
	}
};

} // namespace gfn::core::data