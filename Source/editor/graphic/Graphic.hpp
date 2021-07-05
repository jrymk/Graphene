#pragma once

#include <vector>
#include <atomic>
#include "../../core/Vector.hpp"
#include "../../core/Vertex.hpp"
#include "../../core/Edge.hpp"

// the barebones graph representation for rendering
namespace graphene::editor::graphic {
class Graphic {
  public:
	struct Vertex {
		::graphene::core::Vertex* corePtr;
		Vec2f position;
	};
	struct Edge {
		::graphene::core::Edge* corePtr;
	};

	std::atomic<bool> writeReady;
	std::vector<Edge> edges;
	std::vector<Vertex> vertices;

	Graphic() = default;

	void clear() {
		edges.clear();
		vertices.clear();
	}
};
} // namespace graphene::editor::graphic