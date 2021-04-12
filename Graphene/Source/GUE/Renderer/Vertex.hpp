#pragma once
#include "Structures.hpp"

namespace gue {
	struct Vertex {
		Vec2f position;
		Colorf color;

		Vertex(Vec2f position, Colorf color) {
			this->position = position;
			this->color = color;
		}
		
	};
}