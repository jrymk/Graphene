#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "../editor/graphic/DoubleBuffer.hpp"

namespace graphene::core {
class Core;

class GraphicExporter {
  public:
	Core* core;

	GraphicExporter(Core* core_) : core(core_) {}

	void
	exportGraphic(std::unordered_map<Vertex*, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>>& graph) {
		if (graphene::editor::graphic::backBuffer->writeReady) {
			graphene::editor::graphic::swapReady = false;
			graphene::editor::graphic::backBuffer->clear();

			for (auto v : graph) {
				graphene::editor::graphic::backBuffer->vertices.push_back(graphene::editor::graphic::Graphic::Vertex());
				graphene::editor::graphic::backBuffer->vertices.rbegin()->corePtr = v.first;
				graphene::editor::graphic::backBuffer->vertices.rbegin()->position = v.first->getCoord();
			}

			graphene::editor::graphic::swapReady = true;
			graphene::editor::graphic::backBufferUpdated = true;
			// LOG_DEBUG("core thread swap buffer successful");
		}
	}
};
} // namespace graphene::core
