#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <Core/Data/Uuid.hpp>
#include <Core/Data/Properties/Properties.hpp>
#include <Core/Data/Structure/ConnectedComponent.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Tasks/Tasks.hpp>

namespace gfn::core::data {

/**
 * A biconnected component consists of several vertices including articulation,
 * but articulation will be in an independent object here, and the remaining parts
 * will be in the same object with the type BLOCK.
 *
 * By definition, adjacent articulations share a biconnected component without
 * non-articulation vertices, and the block will have no vertices, so note
 * that 'vertices' may be empty.
 */

class BlockCutTree {
  private:
	gfn::core::data::Properties* properties;
	gfn::core::data::ConnectedComponent* component;

  public:
	std::unordered_map<gfn::core::Uuid, std::vector<gfn::core::Uuid>>
		vertices;												 // list of vertices in a given biconnected component
	std::unordered_map<gfn::core::Uuid, gfn::core::Uuid> vToBcc; // vertex to its corresponding biconnected component
	std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>
		blockCutTree; // the adjacency list representation of the BCT
	std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>> bccMapping; // maps bcc to all its vertices

	void bindComponent(gfn::core::data::ConnectedComponent* component) { this->component = component; }

	void bindProperties(gfn::core::data::Properties* properties) { this->properties = properties; }

	void addEdge(gfn::core::Uuid u, gfn::core::Uuid v) {
		if (properties->getBiconnectedComponentProp(u).type == properties->getBiconnectedComponentProp(v).type)
			logWarning("core::data::BlockCutTree: Adding edge between biconnected component failed: same type");
		else {
			blockCutTree[u].insert(v);
			blockCutTree[v].insert(u);
		}
	}

	class Builder {
	  private:
		gfn::core::data::Properties* properties;
		BlockCutTree* bct;

	  public:
		void build() {
			bct->vToBcc.clear();
			bct->blockCutTree.clear();
			bct->bccMapping.clear();

			visited.clear();
			children.clear();
			in.clear();
			low.clear();
			articulation.clear();
			timeStamp = 0;
			while (!vertexStack.empty())
				vertexStack.pop();

			if (bct->vToBcc.empty()) {
				return;
			}
			auto root = bct->component->root;

			countChildren(root);
			dfs(root, root);

			// stores the bcc to vertices mapping (actual)
			buildMapping();
		}

	  private:
		std::unordered_set<gfn::core::Uuid> visited;
		std::unordered_map<gfn::core::Uuid, int> children;
		std::unordered_map<gfn::core::Uuid, int> in, low;
		std::unordered_set<gfn::core::Uuid> articulation;
		int timeStamp = 0;

		std::stack<gfn::core::Uuid> vertexStack;

		void countChildren(gfn::core::Uuid now) {
			//            std::cerr << "count " << now->uid << "\n";
			children[now] = 0;
			visited.insert(now);
			for (gfn::core::Uuid i : bct->component->adjList[now]) {
				if (visited.find(i) != visited.end())
					continue;
				countChildren(i);
				children[now]++;
			}
		}

		gfn::core::Uuid makeBlock(gfn::core::Uuid until = gfn::system::uuid::createNil()) {
			gfn::core::Uuid bcc = gfn::system::uuid::createUuid();
			properties->newBiconnectedComponentProp(bcc, gfn::core::data::BiconnectedComponentProp::Type::BLOCK);
			bct->blockCutTree[bcc];

			while (!vertexStack.empty()) {
				gfn::core::Uuid x = vertexStack.top();
				if (articulation.find(x) == articulation.end()) {
					bct->vToBcc[x] = bcc;
					bct->vertices[bcc].push_back(x);
				} else
					bct->addEdge(bcc, bct->vToBcc[x]);
				vertexStack.pop();
				if (x == until)
					break;
			}
			return bcc;
		}

		gfn::core::Uuid makeArticulation(gfn::core::Uuid now) {
			if (articulation.find(now) != articulation.end()) {
				return bct->vToBcc[now];
			}
			//            std::cerr << "articulation " << now->uid << std::endl;
			articulation.insert(now);
			gfn::core::Uuid bcc = gfn::system::uuid::createUuid();
			properties->newBiconnectedComponentProp(bcc, gfn::core::data::BiconnectedComponentProp::Type::BLOCK);
			bct->blockCutTree[bcc];
			bct->vToBcc[now] = bcc;
			bct->vertices[now].push_back(now);
			return bcc;
		}

		void dfs(gfn::core::Uuid now, gfn::core::Uuid parent) {
			in[now] = low[now] = timeStamp++;
			vertexStack.push(now);
			for (gfn::core::Uuid i : bct->component->adjList[now]) {
				if (i == parent)
					continue;
				if (in.find(i) != in.end()) {
					low[now] = std::min(low[now], in[i]);
				} else {
					dfs(i, now);
					low[now] = std::min(low[now], low[i]);

					if ((now != parent && low[i] >= in[now]) || (now == parent && children[now] > 1)) {
						auto bcc = makeBlock(i);
						auto articulationBCC = makeArticulation(now);
						bct->addEdge(bcc, articulationBCC);
					}
				}
			}
			if (now == parent && children[now] == 1 && articulation.find(now) == articulation.end()) {
				makeBlock();
			}
		}

		void buildMapping() {
			bct->bccMapping.clear();
			for (auto bcc : bct->blockCutTree) {
				if (properties->getBiconnectedComponentProp(bcc.first).type ==
					gfn::core::data::BiconnectedComponentProp::Type::BLOCK)
					bct->bccMapping.insert({bcc.first, std::unordered_set<gfn::core::Uuid>()});
			}
			for (auto v : bct->vToBcc) {
				if (properties->getBiconnectedComponentProp(v.second).type ==
					gfn::core::data::BiconnectedComponentProp::Type::BLOCK)
					bct->bccMapping.find(v.second)->second.insert(v.first);
				else {
					for (auto c : bct->blockCutTree.find(v.second)->second) {
						if (properties->getBiconnectedComponentProp(c).type ==
							gfn::core::data::BiconnectedComponentProp::Type::BLOCK)
							bct->bccMapping.find(c)->second.insert(v.first);
					}
				}
			}
		}
	};
};
} // namespace gfn::core::data