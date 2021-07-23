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
class BlockCutTree;

class ConnectedComponent {
  public:
	gfn::core::Uuid uuid;
	gfn::core::Uuid root;
	std::unordered_set<gfn::core::Uuid> vertices;
	std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>> adjList;
	gfn::core::data::BlockCutTree bcc;
	gfn::core::data::Properties* properties;

	ConnectedComponent(gfn::core::Uuid _uuid, gfn::core::data::Properties* _properties)
		: uuid(_uuid), properties(_properties) {
		bcc.bindProperties(_properties);
		bcc.bindComponent(this);
	}
};

class ComponentList {
  private:
	gfn::core::TaskManager* taskManager;
	gfn::core::data::Properties* properties;
	gfn::core::data::AdjacencyList* source;

	std::unordered_map<gfn::core::Uuid, gfn::core::data::ConnectedComponent> components;

	void updateDfs(gfn::core::data::ConnectedComponent& component, gfn::core::Uuid u,
				   std::unordered_set<gfn::core::Uuid>& remaining) {
		remaining.erase(u);
		component.vertices.insert(u);
		component.adjList[u];
		properties->getVertexProp(u).component = component.uuid;

		auto uIt = source->getRawList().find(u);
		if (uIt == source->getRawList().end()) {
			logInsert("core::data::ComponentList: update component list failed to access vID ");
			logInsert(u);
			logWarning(", the following vertex will be ignored");
		}

		for (auto& v : uIt->second) {
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
			components.insert(std::make_pair(uuid, gfn::core::data::ConnectedComponent(uuid, properties)));

			updateDfs(components.find(uuid)->second, *vIt, remaining);
		}

		// build the in-component adjacency list
		for (auto& u : source->getRawList()) {
			for (auto& v : u.second) {
				components.find(properties->getVertexProp(u.first).component)->second.adjList[u.first].insert(v.first);
				components.find(properties->getVertexProp(v.first).component)->second.adjList[v.first].insert(u.first);
			}
		}
	}
};

} // namespace gfn::core::data